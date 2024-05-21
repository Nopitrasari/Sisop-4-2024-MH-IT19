#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <sys/stat.h>
#include <dirent.h> // Add this line

//Punya Benjamin NRP 078
//buat static const
static const char *dirpath = "/home/benyamin/fuse_fs";

static int hello_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    int res;

    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}


static int hello_mkdir(const char *path, mode_t mode)
{
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    int res;

    res = mkdir(fpath, mode);
    if(res == -1)
        return -errno;

    return 0;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);

    DIR *dp;
    struct dirent *de;

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL) { //buat read direktori 
         struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static struct fuse_operations hello_oper = {
    .getattr = hello_getattr,
    .mkdir = hello_mkdir,
    .readdir = hello_readdir,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &hello_oper, NULL);
}



gcc -Wall `pkg-config fuse --cflags` fuse.c -o fuse_output `pkg-config fuse --libs`
