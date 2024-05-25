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
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#define PATH_MAX_LENGTH 1024

static const char *gallery_path = "/gallery";
static const char *bahaya_path = "/bahaya";

static int gallery_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                           off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    if (strcmp(path, gallery_path) != 0)
        return -ENOENT;

    DIR *directory = opendir("gallery");
    if (directory == NULL)
        return -errno;

    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        if (filler(buf, entry->d_name, NULL, 0))
            break;
    }

    closedir(directory);
    return 0;
}

static int bahaya_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                          off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    if (strcmp(path, bahaya_path) != 0)
        return -ENOENT;

    DIR *directory = opendir("bahaya");
    if (directory == NULL)
        return -errno;

    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        if (filler(buf, entry->d_name, NULL, 0))
            break;
    }

    closedir(directory);
    return 0;
}

static int gallery_getattr(const char *path, struct stat *stbuf) {
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, gallery_path) == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        res = -ENOENT;
    }

    return res;
}

static int bahaya_getattr(const char *path, struct stat *stbuf) {
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, bahaya_path) == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        res = -ENOENT;
    }

    return res;
}

static struct fuse_operations gallery_oper = {
    .getattr = gallery_getattr,
    .readdir = gallery_readdir,
};

static struct fuse_operations bahaya_oper = {
    .getattr = bahaya_getattr,
    .readdir = bahaya_readdir,
};

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &gallery_oper, NULL);
}

```
# DOKUMENTASI
![Screenshot from 2024-05-25 20-35-17](https://github.com/Nopitrasari/Sisop-4-2024-MH-IT19/assets/151106171/ebd4dd93-d1a3-4aa4-8b5d-4e79f4a1ba10)

![Screenshot from 2024-05-25 20-35-31](https://github.com/Nopitrasari/Sisop-4-2024-MH-IT19/assets/151106171/528207fb-50a1-4327-b62a-9afe640b0e6d)

![Screenshot from 2024-05-25 20-35-52](https://github.com/Nopitrasari/Sisop-4-2024-MH-IT19/assets/151106171/97f893af-0ecb-4f6f-ab19-ade05b887d0f)

![Screenshot from 2024-05-25 20-36-26](https://github.com/Nopitrasari/Sisop-4-2024-MH-IT19/assets/151106171/6ec3839d-f1c2-4f53-9f33-9898a431ee46)

PADA BAGIAN MOUNT_POINT :

![image](https://github.com/Nopitrasari/Sisop-4-2024-MH-IT19/assets/151106171/afe6f909-1a1c-4c01-9524-c21340bdd04d)

SUDAH BISA TETAPI MASIH KOSONG MAS, DISINI FUSE SUDAH TERINSTALL TAPI MASIH TERLIHAT SEAKAN GA KEINSTALL KENDALANYA DISITU. SUDAH SAYA COBA COBA TAPI MASIH STUCK DISITU.

# SOAL 2

Masih dengan Ini Karya Kita, sang CEO ingin melakukan tes keamanan pada folder sensitif Ini Karya Kita. Karena Teknologi Informasi merupakan departemen dengan salah satu fokus di Cyber Security, maka dia kembali meminta bantuan mahasiswa Teknologi Informasi angkatan 2023 untuk menguji dan mengatur keamanan pada folder sensitif tersebut. Untuk mendapatkan folder sensitif itu, mahasiswa IT 23 harus kembali mengunjungi website Ini Karya Kita pada www.inikaryakita.id/schedule . Silahkan isi semua formnya, tapi pada form subject isi dengan nama kelompok_SISOP24 , ex: IT01_SISOP24 . Lalu untuk form Masukkan Pesanmu, ketik “Mau Foldernya” . Tunggu hingga 1x24 jam, maka folder sensitif tersebut akan dikirimkan melalui email kalian. Apabila folder tidak dikirimkan ke email kalian, maka hubungi sang CEO untuk meminta bantuan.   

Pada folder "pesan" Adfi ingin meningkatkan kemampuan sistemnya dalam mengelola berkas-berkas teks dengan menggunakan fuse.

Jika sebuah file memiliki prefix "base64," maka sistem akan langsung mendekode isi file tersebut dengan algoritma Base64.

Jika sebuah file memiliki prefix "rot13," maka isi file tersebut akan langsung di-decode dengan algoritma ROT13.

Jika sebuah file memiliki prefix "hex," maka isi file tersebut akan langsung di-decode dari representasi heksadesimalnya.

Jika sebuah file memiliki prefix "rev," maka isi file tersebut akan langsung di-decode dengan cara membalikkan teksnya.

Contoh:

File yang belum didecode/ dienkripsi rot_13


File yang sudah didecode/ dienkripsi rot_13


Pada folder “rahasia-berkas”, Adfi dan timnya memutuskan untuk menerapkan kebijakan khusus. Mereka ingin memastikan bahwa folder dengan prefix "rahasia" tidak dapat diakses tanpa izin khusus. 
Jika seseorang ingin mengakses folder dan file pada “rahasia”, mereka harus memasukkan sebuah password terlebih dahulu (password bebas). 

Setiap proses yang dilakukan akan tercatat pada logs-fuse.log dengan format :

[SUCCESS/FAILED]::dd/mm/yyyy-hh:mm:ss::[tag]::[information]

Ex:

[SUCCESS]::01/11/2023-10:43:43::[moveFile]::[File moved successfully]

Catatan: 

Tolong cek email kalian secara berkala untuk mendapatkan folder sensitifnya
Jika ada pertanyaan, bisa menghubungi Adfi via WA aja, nanti dikasih hint

# REVISI
```
static void log_event(const char *tag, const char *information, int success) {
    time_t now;
    struct tm *local_time;
    char timestamp[20];

    time(&now);
    local_time = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%d/%m/%Y-%H:%M:%S", local_time);

    FILE *log_file = fopen(log_file_path, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Error opening log file %s: %s\n", log_file_path, strerror(errno));
        return;
    }

    fprintf(log_file, "[%s]::%s::[%s]::[%s]\n", success ? "SUCCESS" : "FAILED", timestamp, tag, information);
    fclose(log_file);
}
```
kode diatas adalah fungsi untuk membuat file log. dimana file log akan berada di folder sisop

![image](https://github.com/Nopitrasari/Sisop-4-2024-MH-IT19/assets/151911480/9d4ad43d-8c99-4dd5-ba37-35bfece1cda6)

berikut adalah isi dari file log tersebut

```
static void decrypt_rot13(char *str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        if (isalpha(str[i])) {
            char base = isupper(str[i]) ? 'A' : 'a';
            str[i] = (((str[i] - base) - 13 + 26) % 26) + base;
        }
    }
}
```
kode diatas adalah deskripsi untuk file rot13

dibawah ini adalah file rot13 sebelum di mountpoint
```
Zngn xhyvnu fvfgrz bcrenfv zrehcnxna fnynu fngh xbzcbara shaqnzragny qnynz fghqv grxabybtv vasbeznfv qna vyzh xbzchgre. Zngn xhyvnu vav zrzsbxhfxna cnqn crznunzna qna crarencna xbafrc-xbafrc qnfne lnat orexnvgna qratna cratrybynna fhzore qnln xbzchgre, frcregv znanwrzra zrzbev, znanwrzra cebfrf, qna znanwrzra svyr. Qnynz xhefhf vav, znunfvfjn nxna zrzcrynwnev ontnvznan fvfgrz bcrenfv orecrena qnynz zratbbeqvanfvxna oreontnv nxgvivgnf xbzchgre, zratbcgvznyxna xvarewn, qna zrzorevxna yvatxhatna lnat fgnovy ontv ncyvxnfv. Frynva vgh, znunfvfjn whtn nxna qvnwnx haghx zrznunzv cevafvc-cevafvc qrfnva fvfgrz bcrenfv fregn zratrzonatxna xrgrenzcvyna qnynz crzrpnuna znfnynu grexnvg qratna fvfgrz bcrenfv. Xrfryhehuna, zngn xhyvnu fvfgrz bcrenfv zrzvyvxv crena cragvat qnynz zrzorxnyv znunfvfjn qratna cratrgnuhna lnat qvcreyhxna haghx zrenapnat, zratrybyn, qna zrznunzv xrewn fvfgrz xbzchgre frpnen rsvfvra.
```

dan dibawah ini adalah hasil setelah di mountpoint
```
Mata kuliah sistem operasi merupakan salah satu komponen fundamental dalam studi teknologi informasi dan ilmu komputer. Mata kuliah ini memfokuskan pada pemahaman dan penerapan konsep-konsep dasar yang berkaitan dengan pengelolaan sumber daya komputer, seperti manajemen memori, manajemen proses, dan manajemen file. Dalam kursus ini, mahasiswa akan mempelajari bagaimana sistem operasi berperan dalam mengoordinasikan berbagai aktivitas komputer, mengoptimalkan kinerja, dan memberikan lingkungan yang stabil bagi aplikasi. Selain itu, mahasiswa juga akan diajak untuk memahami prinsip-prinsip desain sistem operasi serta mengembangkan keterampilan dalam pemecahan masalah terkait dengan sistem operasi. Keseluruhan, mata kuliah sistem operasi memiliki peran penting dalam membekali mahasiswa dengan pengetahuan yang diperlukan untuk merancang, mengelola, dan memahami kerja sistem komputer secara efisien.
```

```
static void decode_hex(const char *input, char *output) {
    size_t len = strlen(input) / 2;
    for (size_t i = 0; i < len; i++) {
        sscanf(input + 2 * i, "%2hhx", &output[i]);
    }
    output[len] = '\0';
}
```
berikut adalah kode untuk deskripsi file hex

dibawah ini adalah contoh file hex sebelum di jalankan
```
73656d616e67617420796177
```

dan dibawah ini adalah hasi file hex setelah dijalankan
```
semangat yaw
```

```
static void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}
```

kode diatas adalah kode untuk deskripsi file reverse

berikut adalah contoh file reverse

sebelum
```
asib kok asib ,apapaG
```

setelah
```
Gapapa, bisa kok bisa
```

```
static void decode_base64(const char *input, char *output) {
    int length = strlen(input);
    int decoded_length = length / 4 * 3;
    if (input[length - 1] == '=') decoded_length--;
    if (input[length - 2] == '=') decoded_length--;

    unsigned char *decoded_data = malloc(decoded_length + 1);
    if (!decoded_data) return;

    for (int i = 0, j = 0; i < length;) {
        uint32_t sextet_a = input[i] == '=' ? 0 & i++ : strchr(base64_table, input[i++]) - base64_table;
        uint32_t sextet_b = input[i] == '=' ? 0 & i++ : strchr(base64_table, input[i++]) - base64_table;
        uint32_t sextet_c = input[i] == '=' ? 0 & i++ : strchr(base64_table, input[i++]) - base64_table;
        uint32_t sextet_d = input[i] == '=' ? 0 & i++ : strchr(base64_table, input[i++]) - base64_table;
        uint32_t triple = (sextet_a << 18) | (sextet_b << 12) | (sextet_c << 6) | sextet_d;

        if (j < decoded_length) decoded_data[j++] = (triple >> 16) & 0xFF;
        if (j < decoded_length) decoded_data[j++] = (triple >> 8) & 0xFF;
        if (j < decoded_length) decoded_data[j++] = triple & 0xFF;
    }

    decoded_data[decoded_length] = '\0';
    strcpy(output, (char *)decoded_data);
    free(decoded_data);
}
```

sedangkan kode diatas untuk deskripsi file base64

berikut adalah contohnya

sebelum
```
TWFrYXNpaCB5YSB1ZGFoIGJlcnVzYWhh
```

setelah
```
Makasih ya udah berusaha
```

dibawah ini adalah kode untuk deskripsi isi dari log file
```
static void decrypt_message(const char *path, char *buf) {
    char *temp_buf = strdup(buf);
    if (!temp_buf) return;

    if (strstr(path, "rev") != NULL) {
        reverse_string(temp_buf);
        log_event("moveFile", "File moved successfully", 1);
    } else if (strstr(path, "hex") != NULL) {
        decode_hex(temp_buf, buf);
        free(temp_buf);
        log_event("moveFile", "File moved successfully", 1);
        return;
    } else if (strstr(path, "base64") != NULL) {
        decode_base64(temp_buf, buf);
        free(temp_buf);
        log_event("moveFile", "File moved successfully", 1);
        return;
    } else if (strstr(path, "rot13") != NULL) {
        decrypt_rot13(temp_buf);
        log_event("moveFile", "File moved successfully", 1);
    } else {
        log_event("moveFile", "File moved successfully", 1);
    }

    strcpy(buf, temp_buf);
    free(temp_buf);
}
```

selanjutnya yaitu kode untuk fungsi gettatr
```
static int fs_getattr(const char *path, struct stat *stbuf) {
    int res = 0;
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        char full_path[1000];
        snprintf(full_path, sizeof(full_path), "%s%s", source_dir, path);
        res = lstat(full_path, stbuf);
        if (res == -1) {
            return -errno;
        }
    }
    return res;
}
```

dan dibawah ini adalah fungsi untuk readdir
```
static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", source_dir, path);

    DIR *dp = opendir(full_path);
    if (dp == NULL) {
        return -errno;
    }

    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
        if (filler(buf, de->d_name, NULL, 0)) break;
    }

    closedir(dp);
    return 0;
}
```

selanjutnya yaitu fungsi untuk membuka file yang sudah dimounpoint
```
static int fs_open(const char *path, struct fuse_file_info *fi) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", source_dir, path);

    int fd = open(full_path, fi->flags);
    if (fd == -1) {
        return -errno;
    }

    close(fd);
    return 0;
}

```

selanjutnya yaitu fungsi untuk read
```
static int fs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", source_dir, path);

    int fd = open(full_path, O_RDONLY);
    if (fd == -1) {
        return -errno;
    }

    if (lseek(fd, offset, SEEK_SET) == -1) {
        close(fd);
        return -errno;
    }

    char *read_buf = malloc(size + 1);
    if (!read_buf) {
        close(fd);
        return -ENOMEM;
    }

    ssize_t bytes_read = read(fd, read_buf, size);
    if (bytes_read == -1) {
        close(fd);
        free(read_buf);
        return -errno;
    }

    read_buf[bytes_read] = '\0';


    decrypt_message(path, read_buf);

    size_t decrypted_length = strlen(read_buf);
    if (decrypted_length > size) decrypted_length = size;

    memcpy(buf, read_buf, decrypted_length);
    free(read_buf);
    close(fd);
    return decrypted_length;
}
```

selanjutnya yaitu fungsi untuk rename
```
static int fs_rename(const char *from, const char *to) {
    char full_from[1000];
    char full_to[1000];
    snprintf(full_from, sizeof(full_from), "%s%s", source_dir, from);
    snprintf(full_to, sizeof(full_to), "%s%s", source_dir, to);

    int res = rename(full_from, full_to);
    if (res == -1) {
        log_event("moveFile", "Failed to move file", 0);
        return -errno;
    }

    log_event("moveFile", "File moved successfully", 1);
    return 0;
}
```

dan selanjutnya yaitu fuse operations dan juga fungsi mainnya
```
static struct fuse_operations fs_ops = {
    .getattr = fs_getattr,
    .readdir = fs_readdir,
    .open = fs_open,
    .read = fs_read,
    .rename = fs_rename,
};

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &fs_ops, NULL);
}
```

dan dibawah ini adalah fungsi untuk letak filenya dan juga tabel base64
```
static const char *source_dir = "/home/riskiya/sisop/sensitif";
static const char *log_file_path = "/home/riskiya/sisop/fuse-log.txt";
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
```

dan ini bukti bahwa mountpointnya sudah berhasil

![image](https://github.com/Nopitrasari/Sisop-4-2024-MH-IT19/assets/151911480/a0aa35df-acdd-4a81-9f83-1cf4ed1089fd)



untuk file rahasia-berkas tidak ada perubahan

#SOAL 3

Seorang arkeolog menemukan sebuah gua yang didalamnya tersimpan banyak relik dari zaman praaksara, sayangnya semua barang yang ada pada gua tersebut memiliki bentuk yang terpecah belah akibat bencana yang tidak diketahui. Sang arkeolog ingin menemukan cara cepat agar ia bisa menggabungkan relik-relik yang terpecah itu, namun karena setiap pecahan relik itu masih memiliki nilai tersendiri, ia memutuskan untuk membuat sebuah file system yang mana saat ia mengakses file system tersebut ia dapat melihat semua relik dalam keadaan utuh, sementara relik yang asli tidak berubah sama sekali.
Ketentuan :
1. Buatlah sebuah direktori dengan ketentuan seperti pada tree berikut
   
```
├── [nama_bebas]
├── relics
│   ├── relic_1.png.000
│   ├── relic_1.png.001
│   ├── dst dst…
│   └── relic_9.png.010
└── report
```

2. Direktori [nama_bebas] adalah direktori FUSE dengan direktori asalnya adalah direktori relics. Ketentuan Direktori [nama_bebas] adalah sebagai berikut :
Ketika dilakukan listing, isi dari direktori [nama_bebas] adalah semua relic dari relics yang telah tergabung.

- Ketika dilakukan copy (dari direktori [nama_bebas] ke tujuan manapun), file yang disalin adalah file dari dir- ektori relics yang sudah tergabung.

- Ketika ada file dibuat, maka pada direktori asal (direktori relics) file tersebut akan dipecah menjadi sejumlah pecahan dengan ukuran maksimum tiap pecahan adalah 10kb.

- File yang dipecah akan memiliki nama [namafile].000 dan seterusnya sesuai dengan jumlah pecahannya.
	Ketika dilakukan penghapusan, maka semua pecahannya juga ikut terhapus.

3. Direktori report adalah direktori yang akan dibagikan menggunakan Samba File Server. Setelah kalian berhasil membuat direktori [nama_bebas], jalankan FUSE dan salin semua isi direktori [nama_bebas] pada direktori report.


- Catatan:
	pada contoh terdapat 20 relic, namun pada zip file hanya akan ada 10 relic
	[nama_bebas] berarti namanya dibebaskan
	pada soal 3c, cukup salin secara manual. File Server hanya untuk membuktikan bahwa semua file pada direktori [nama_bebas] dapat dibuka dengan baik.

- discoveries.zip

