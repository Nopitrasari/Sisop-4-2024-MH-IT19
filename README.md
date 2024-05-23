# Sisop-4-2024-MH-IT19
Anggota Kelompok IT19 :

1. Riskiyatul Nur Oktarani (5027231013)
2. Aswalia Novitriasari (5027231012)
3. Benjamin Khawarizmi Habibi (5027231078)

# DESKRIPSI PRAKTIKUM SOAL SHIFT MODUL 4

# SOAL 1
Adfi merupakan seorang CEO agency creative bernama Ini Karya Kita. Ia sedang melakukan inovasi pada manajemen project photography Ini Karya Kita. Salah satu ide yang dia kembangkan adalah tentang pengelolaan foto project dalam sistem arsip Ini Karya Kita. Dalam membangun sistem ini, Adfi tidak bisa melakukannya sendirian, dia perlu bantuan mahasiswa Departemen Teknologi Informasi angkatan 2023 untuk membahas konsep baru yang akan mengubah project fotografinya lebih menarik untuk dilihat. Adfi telah menyiapkan portofolio hasil project fotonya yang bisa didownload dan diakses di www.inikaryakita.id . Silahkan eksplorasi web Ini Karya Kita dan temukan halaman untuk bisa mendownload projectnya. Setelah kalian download terdapat folder gallery dan bahaya.
Pada folder “gallery”:
Membuat folder dengan prefix "wm." Dalam folder ini, setiap gambar yang dipindahkan ke dalamnya akan diberikan watermark bertuliskan inikaryakita.id. 
			Ex: "mv ikk.jpeg wm-foto/" 
Output:

Before: (tidak ada watermark bertuliskan inikaryakita.id)

After: (terdapat watermark tulisan inikaryakita.id)

Pada folder "bahaya," terdapat file bernama "script.sh." Adfi menyadari pentingnya menjaga keamanan dan integritas data dalam folder ini. 
Mereka harus mengubah permission pada file "script.sh" agar bisa dijalankan, karena jika dijalankan maka dapat menghapus semua dan isi dari  "gallery"
Adfi dan timnya juga ingin menambahkan fitur baru dengan membuat file dengan prefix "test" yang ketika disimpan akan mengalami pembalikan (reverse) isi dari file tersebut.  

# PENYELESAIAN
1) Jadi kita unduh file portofolio.zip dari www.inikaryakita.id
2) lalu unzip dan ekstrak dengan command :
   ``` unzip portofolio.zip -d Soal_1 ```
   jadi sebelum nya kita udah buat direktori Soal_1
3) lalu kita membuat file inikaryakita.c berikut adalah isi nya :
```
#define FUSE_USE_VERSION 35

#include <fuse3/fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <libgen.h>

#define GALLERY_DIR "gallery"
#define WM_DIR "gallery/wm-foto"
#define BAHAYA_DIR "bahaya"
#define SCRIPT_PATH "bahaya/script.sh"
#define REVERSE_FILE "bahaya/test-adfi.txt"

//UNTUK MENAMBAHKAN WATERMARK DAN MENGEKSTRAK FOTO
static void add_watermark_and_move(const char *filename) {
    char command[1024];
    snprintf(command, sizeof(command), "convert '%s' -gravity southeast -draw \"text 0,0 'inikaryakita.id'\" '%s/%s'", filename, WM_DIR, basename((char *)filename));
    if (system(command) == 0) {
        printf("File %s has been watermarked and moved to %s\n", filename, WM_DIR);
    } else {
        printf("Failed to watermark and move file %s\n", filename);
    }
}

//MENGUBAH IZIN FILE SCRIPT.SH
static void make_script_executable(const char *script_path) {
    if (chmod(script_path, S_IRUSR | S_IWUSR | S_IXUSR) == 0) {
        printf("Permissions for %s changed to executable\n", script_path);
    } else {
        perror("chmod");
    }
}

//MEREVERSE FILE TEST-ADFI
static void reverse_file(const char *filepath) {
    char command[4096];
    snprintf(command, sizeof(command), "rev '%s' > '%s/test_%s'", filepath, BAHAYA_DIR, basename((char *)filepath));
    if (system(command) == 0) {
        printf("File %s has been reversed and saved as test_%s\n", filepath, basename((char *)filepath));
    } else {
        printf("Failed to reverse %s\n", filepath);
    }
}

static int do_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    (void) fi;

    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s%s", GALLERY_DIR, path);
        res = lstat(fullpath, stbuf);
        if (res == -1)
            return -errno;
    }

    return res;
}
 //UNTUK FUSE NYA
static int do_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
                      struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
    (void) offset;
    (void) fi;
    (void) flags;

    if (strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);

    DIR *dp;
    struct dirent *de;

    dp = opendir(GALLERY_DIR);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static struct fuse_operations operations = {
    .getattr    = do_getattr,
    .readdir    = do_readdir,
};

int main(int argc, char *argv[]) {
    // Make wm-foto directory if it doesn't exist
    if (mkdir(WM_DIR, 0777) == -1 && errno != EEXIST) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }

    // Add watermark and move photos from gallery to wm-foto directory
    DIR *gallery_dp;
    struct dirent *gallery_de;

    gallery_dp = opendir(GALLERY_DIR);
    if (gallery_dp == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((gallery_de = readdir(gallery_dp)) != NULL) {
        if (gallery_de->d_type == DT_REG) {
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", GALLERY_DIR, gallery_de->d_name);
            add_watermark_and_move(filepath);
        }
    }

    closedir(gallery_dp);

    // Make script.sh executable
    make_script_executable(SCRIPT_PATH);

    // Reverse the content of a file and save it as test_<original_file_name>
    reverse_file(REVERSE_FILE);

    return fuse_main(argc, argv, &operations, NULL);
} 
``` 
