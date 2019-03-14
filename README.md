# SoalShift_modul2_E04

3. 
Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
i)  mengekstrak file zip tersebut.
ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 
Catatan:  
-Gunakan fork dan exec.
-Gunakan minimal 3 proses yang diakhiri dengan exec.
-Gunakan pipe
-Pastikan file daftar.txt dapat diakses dari text editor

Jadi dalam soal ini, dibutuhkan 3 proses. Proses yang harus dijalankan pertama adalah proses untuk mengekstrak file campur2.zip. Proses ini dijalankan di proses grandchild. Grandchild merupakan fork dari proses child. Child merupakan fork dari parent process. Parent akan menunggu child dan child menunggu grandchild. Oleh karena itu ekstrak file ditaruh di proses grandchild. Syntax untuk melakukan unzip adalah:

char *argv[4] = {"unzip", "-oq",args[1],NULL};
execv("/usr/bin/unzip", argv);

Kemudian, dalam proses child, akan dilakukan touch untuk membuat folder bernama daftar.txt. Setelah unzip selesai, maka akan dijalankan proses untuk membuat file bernama daftar.txt ini. Daftar.txt digunakan untuk menampung daftar file dari hasil ekstrak yang telah dilakukan di grandchild process. Oleh karena itu, pembuatan file ini dilakukan setelah ekstrak campur2.zip. Syntax untuk melakukannya adalah:

 while ((wait(&statusgrandchild)) > 0);
 
 char *argv[] = {"touch", "daftar.txt", NULL};
 execv("/usr/bin/touch", argv);
 
 Kemudian, untuk mendapatkan daftar file dari folder campur2, digunakan syntax pipe and file. 
 
 fp = popen("ls campur2/*.txt | awk 'BEGIN {FS=\"/\"} {print $2}'","r");
 
 popen adalah pipe open . ls campur2/*.txt adalah untuk mencari file yang berextension.txt dan kemudian menggunakan awk untuk print kolom ke 2.
 
 fp2 = fopen("daftar.txt", "w");

      while (fgets(path, 50, fp))
      {
         fprintf(fp2,"%s\n",path);
      }
fclose(fp2);
pclose(fp);

fopen untuk membuka daftar.txt dan fungsi while dibawahnya adalah untuk menaruh nama-nama file yang ada di folder campur2 di daftar.txt dan melakukan fclose setelah selesai. Jangan lupa juga lakukan pclose untuk menutup pipe.

char *argv[] = {"chmod", "666","daftar.txt", NULL};
execv("bin/chmod",argv);

chmod untuk mencukupi kebutuhan soal untuk melakukan exec 3 kali.

4.

 


