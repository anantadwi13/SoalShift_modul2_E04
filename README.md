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
Selanjutnya, digunakan time_t dan ctime untuk mendapatkan waktu sekarang dan dimasukkan ke dalam array. Hanya jam dan tanggal saja yang diambil dan dimasukkan ke dalam array. Syntaxnya adalah

``` c
time_t t = time(NULL);
char *waktu1 = ctime(&t);

int i;
for (i=0; i<=11; i++)
{
   if (i==11) temp1[i]='\0';
   else temp1[i] = waktu1[strlen(waktu1)-17+i];
}
```

Mengapa strlen(waktu1) - 17 + i? karena untuk mendapatkan posisi tanggal dan seterusnya kebelakang yaitu jam menit dan sekon.
Kemudian setelah itu, menggunakan struct stat dan st_atime untuk mendapatkan access time dari suatu file. Syntaxnya adalah 
``` c
stat("/home/arisatox/Documents/makanan/makan_enak.txt", &details);
char *waktu2 = ctime(&details.st_atime);
 ```
Kemudian dilakukan looping lagi seperti temp1 di temp2. Bedanya adalah yang ini adalah access time nya.
```c 
for (i=0; i<=11; i++)
{
   if (i==11) temp2[i]='\0';
   else temp2[i] = waktu1[strlen(waktu2)-17+i];
}
```
Kemudian, dihitung perbedaan harinya, dicek apakah beda hari atau tidak. Jika beda hari maka akan ditandai daydif = 1
```c
int daydif;
daydif = ((temp1[0]-'0')*10) + (temp1[1]-'0') - ((temp2[0]-'0')*10) - (temp2[1]-'0');
```
Penghitungannya seperti itu karena diambil dari array, misal 2 dan 4, maka 2 * 10 + 4 sehingga menjadi 24. Tempnya dikurangi '0' karena diubah dari char menjadi int.
```c
dif1=((temp1[3]-'0')*10) + (temp1[4]-'0') - ((temp2[3]-'0')*10) - (temp2[4]-'0');

dif2=((temp1[6]-'0')*10) + (temp1[7]-'0') - ((temp2[6]-'0')*10) - (temp2[7]-'0');

dif3=((temp1[9]-'0')*10) + (temp1[10]-'0') - ((temp2[9]-'0')*10) - (temp2[10]-'0');

int second = dif1*3600 + dif2*60 + dif3;
```
Kemudian setelah itu, dihitung caranya seperti tadi namun untuk jam ,menit ,dan sekon. Setelah itu, semuanya akan dijadikan menjadi sekon. 

Kemudian jika harinya sama dan beda sekonnya <= 30, maka akan dilakukan program untuk membuat file sesuai angka counter yang akan diincrement terus menerus.
```c
if (daydif == 0 && second <= 30)
{
   char temp3[50];
   counter++;
   sprintf(temp3,"/home/arisatox/Documents/makanan/makan_sehat%d.txt",counter);
   FILE *fp;
   fp = fopen(temp3, "a");
   fclose(fp);
}
```
Kemudian tambahkan sleep(5) untuk menjalankan daemon setiap 5 detik .

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
