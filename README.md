# SoalShift_modul2_E04

## No 1
### Soal
Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.

Catatan : Tidak boleh menggunakan crontab.
### Jawab 
Membuat daemon yang akan menjalankan perintah berikut setiap 10 detik :
1. Membuat direktori baru pada destination path
2. Mengecek apakah file dengan ekstensi .png ada di folder source
3. Jika ada maka program akan mengambil nama file tersebut lalu menghapus string `.png` dan menggantikannya dengan string `_grey.png`
4. Setelah itu program akan mengcopy file dari nama file semula dan menempatkannya ke destination path dengan menggunakan nama dari string pada nomor 3

Usage :
``` bash
./soal1 /path/images/source /path/images/destination
```

## No 2
### Soal
Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.

Catatan: Tidak boleh menggunakan crontab

### Jawab
Membuat daemon yang akan menjalankan perintah berikut setiap 3 detik :
1. Mengecek apakah file `elen.ku` ada di folder `hatiku`
2. Melihat user dan group dari file tersebut
3. Jika user dan group file tersebut sama dengan `www-data` maka file tersebut akan dihapus

Cara mengganti user dan group file elen.ku :
```bash
sudo chown www-data:www-data /path/to/elen.ku
```
Cara mengganti permission file elen.ku :
```bash
sudo chmod 777 /path/to/elen.ku
```

## No 3
### Soal
Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
1. mengekstrak file zip tersebut.
2. menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 

Catatan:  
- Gunakan fork dan exec.
- Gunakan minimal 3 proses yang diakhiri dengan exec.
- Gunakan pipe
- Pastikan file daftar.txt dapat diakses dari text editor

### Jawab
Jadi dalam soal ini, dibutuhkan 3 proses. Proses yang harus dijalankan pertama adalah proses untuk mengekstrak file campur2.zip. Proses ini dijalankan di proses grandchild. Grandchild merupakan fork dari proses child. Child merupakan fork dari parent process. Parent akan menunggu child dan child menunggu grandchild. Oleh karena itu ekstrak file ditaruh di proses grandchild. Syntax untuk melakukan unzip adalah:
```c
char *argv[4] = {"unzip", "-oq",args[1],NULL};
execv("/usr/bin/unzip", argv);
```
Kemudian, dalam proses child, akan dilakukan touch untuk membuat folder bernama daftar.txt. Setelah unzip selesai, maka akan dijalankan proses untuk membuat file bernama daftar.txt ini. Daftar.txt digunakan untuk menampung daftar file dari hasil ekstrak yang telah dilakukan di grandchild process. Oleh karena itu, pembuatan file ini dilakukan setelah ekstrak campur2.zip. Syntax untuk melakukannya adalah:
```c
while ((wait(&statusgrandchild)) > 0);

char *argv[] = {"touch", "daftar.txt", NULL};
execv("/usr/bin/touch", argv);
```
Kemudian, untuk mendapatkan daftar file dari folder campur2, digunakan syntax pipe and file. 
```c
fp = popen("ls campur2/*.txt | awk 'BEGIN {FS=\"/\"} {print $2}'","r");
```
popen adalah pipe open . ls campur2/*.txt adalah untuk mencari file yang berextension.txt dan kemudian menggunakan awk untuk print kolom ke 2.
```c 
fp2 = fopen("daftar.txt", "w");

while (fgets(path, 50, fp))
{
   fprintf(fp2,"%s\n",path);
}

fclose(fp2);
pclose(fp);
```
fopen untuk membuka daftar.txt dan fungsi while dibawahnya adalah untuk menaruh nama-nama file yang ada di folder campur2 di daftar.txt dan melakukan fclose setelah selesai. Jangan lupa juga lakukan pclose untuk menutup pipe.
```c
char *argv[] = {"chmod", "666","daftar.txt", NULL};
execv("bin/chmod",argv);
```
chmod untuk mencukupi kebutuhan soal untuk melakukan exec 3 kali.

## No 4
### Soal
Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).

Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.

Contoh:
- File makan_enak.txt terakhir dibuka pada detik ke-1
- Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt

Catatan: 
dilarang menggunakan crontab
Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

### Jawab
Dalam soal ini, harus digunakan daemon agar bisa berjalan tiap 5 detik sekali. Pertama-tama dibuat dulu variabel counter untuk nantinya digunakan untuk penamaan file.
Selanjutnya, digunakan time_t dan struct stat untuk mendapatkan waktu sekarang dan waktu akses terakhir. Untuk waktu sekarang menggunakan time_t = time (NULL), sedangkan untuk waktu akses, harus menggunakan stat dulu dan disimpan di details. Ini untuk mendapatkan informasi dari file itu. Kemudian, digunakan st_atime untuk mengambil waktu akses. Untuk mengetahui perbedaan waktunya, menggunakan difftime antara t dan details.st_atime <= 30. Ini akan mengecek apabila perbedaan waktunya lebih kecil atau sama dengan 30 sekon, maka akan menjalankan if didalamnya. 
Jika kondisi terpenuhi, maka akan menambah counter dan melakukan sprintf di array yang telah dibuat sebelumnya yaitu pathFile. Setelah melakukan sprintf, maka dilakukan fopen dan fclose untuk membuat file baru. Untuk membuat file baru, lakukan fopen dengan "w". Tambahkan sleep(5) untuk melakukannya setiap 5 detik.

``` c
 while(1) {
    struct stat details;

    time_t t = time(NULL);

    stat("/home/anantadwi13/Documents/makanan/makan_enak.txt", &details);

    if ((int)difftime(t, details.st_atime)<=30)
    {
      char pathFile[100];
      counter++;
      sprintf(pathFile,"/home/anantadwi13/Documents/makanan/makan_sehat%d.txt",counter);
      FILE *fp;
      fp = fopen(pathFile, "w");
      fclose(fp);
    }
    sleep(5);
  }
```

## No 5
### Soal
Kerjakan poin a dan b di bawah:
- Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
   
   Ket:
   - Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
   - Per menit memasukkan log#.log ke dalam folder tersebut 
   - ‘#’ : increment per menit. Mulai dari 1.
 - Buatlah program c untuk menghentikan program di atas.

NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.
### Jawab
Membuat daemon yang akan menjalankan perintah berikut setiap 1 menit :
1. Setiap 30 menit mengambil waktu saat ini dengan format `dd:MM:yyyy-hh:mm` dan menyimpan ke variable `timestr`
2. Membuat folder baru pada `/path/to/log` dengan nama sesuai isi variable `timestr`
3. Membuat backup dengan cara mengcopy file pada `/var/log/syslog` ke dalam folder yang dibuat pada langkah ke-2

Cara menghentikan program :
1. Mengecek apakah file `/path/to/log/.daemonid` ada atau tidak
2. Jika ada, membaca isi file tersebut lalu menghentikan process yang memiliki pid sesuai dengan isi file tersebut dengan menggunakan `kill(pid, SIGKILL)`
3. Mengosongkan isi file tersebut, menandakan  tidak ada process yang berjalan

Catatan :
- Ketika membuat daemon, pid dari daemon tersebut disimpan kedalam file `/path/to/log/.daemonid`
- Untuk mencegah terbuatnya zombie process, child process akan dimatikan setelah child process selesai berjalan.
