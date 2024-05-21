#define FUSE_USE_VERSION 35
#include <dirent.h>
#include <cstring>
#include <fuse3/fuse.h>
#include <string>
#include <pwd.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <algorithm>


void logfuse(const std::string& status, const std::string& tag, const std::string& info) {
    std::ofstream log_file("/home/riskiya/logs-fuse.log", std::ios_base::app);
    if (log_file.is_open()) {
        std::time_t now = std::time(nullptr);
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%d/%m/%Y-%H:%M:%S", std::localtime(&now));
        log_file << "[" << status << "]::" << timestamp << "::[" << tag << "]::[" << info << "]\n";
        log_file.close();
    }
}

std::string decode_base64(const std::string& input) {
    return input;
}

std::string decode_rot13(const std::string& input) {
    std::string output = input;
    std::transform(output.begin(), output.end(), output.begin(), [](char c) -> char {
        if ('a' <= c && c <= 'z')
            return (c - 'a' + 13) % 26 + 'a';
        if ('A' <= c && c <= 'Z')
            return (c - 'A' + 13) % 26 + 'A';
        return c;
    });
    return output;
}

std::string decode_hex(const std::string& input) {
    std::string output;
    for (size_t i = 0; i < input.length(); i += 2) {
        std::string byte = input.substr(i, 2);
        char chr = (char)(int)strtol(byte.c_str(), nullptr, 16);
        output.push_back(chr);
    }
    return output;
}

std::string decode_reverse(const std::string& input) {
    return std::string(input.rbegin(), input.rend());
}

static std::string folder_path = "/home/riskiya/root/sensitif";

static int do_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi) {
    std::string full_path = folder_path + path;
    int res = lstat(full_path.c_str(), stbuf);
    if (res == -1) {
        return -errno;
    }
    return 0;
}

static int do_readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi, enum fuse_readdir_flags flags) {
    std::string full_path = folder_path + path;
    DIR* dp = opendir(full_path.c_str());
    if (dp == nullptr) {
        return -errno;
    }

    struct dirent* de;
    while ((de = readdir(dp)) != nullptr) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0, static_cast<fuse_fill_dir_flags>(0)))
            break;
    }

    closedir(dp);
    return 0;
}

static int do_open(const char* path, struct fuse_file_info* fi) {
    std::string full_path = folder_path + path;
    int res = open(full_path.c_str(), fi->flags);
    if (res == -1) {
        return -errno;
    }
    close(res);
    return 0;
}

static int do_access(const char* path, int mask) {
    if (strstr(path, "/pesan-rahasia") == path) {
        std::string password;
        std::cout << "masukkan pasword " << path << ": ";
        std::cin >> password;

        if (password != "correct_password") {
            logfuse("FAILED", "access", "Unauthorized access attempt to " + std::string(path));
            return -EACCES;
        }
    }
    return 0;
}

static int do_read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* fi) {
    std::string full_path = folder_path + path;
    std::ifstream file(full_path, std::ios::binary);
    if (!file.is_open()) {
        return -errno;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::string decoded_content;

    if (strstr(path, "base64_") == path + 1) {
        decoded_content = decode_base64(content);
    } else if (strstr(path, "rot13_") == path + 1) {
        decoded_content = decode_rot13(content);
    } else if (strstr(path, "hex_") == path + 1) {
        decoded_content = decode_hex(content);
    } else if (strstr(path, "rev_") == path + 1) {
        decoded_content = decode_reverse(content);
    } else {
        decoded_content = content;
    }

    if (offset < decoded_content.size()) {
        if (offset + size > decoded_content.size()) {
            size = decoded_content.size() - offset;
        }
        memcpy(buf, decoded_content.data() + offset, size);
    } else {
        size = 0;
    }

    return size;
}


static struct fuse_operations operations = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .open = do_open,
    .access = do_access,
    .read = do_read,
};


int main(int argc, char* argv[]) {
    umask(0);
    return fuse_main(argc, argv, &operations, nullptr);
}


