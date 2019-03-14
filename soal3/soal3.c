#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>

int main()
{
   pid_t child_id;
   pid_t grandchild_id;
   int statuschild;
   int statusgrandchild;

   child_id = fork();

   if (child_id == 0) 
   {
      grandchild_id = fork(); 

      if (grandchild_id == 0) 
      {
            char *argv[4] = {"unzip","campur2.zip",NULL};
            execv("/usr/bin/unzip", argv);
      }
      else
      {

        while ((wait(&statusgrandchild)) > 0);

        char *argv[] = {"touch", "daftar.txt", NULL};
        execv("/usr/bin/touch", argv);    

      }
   } 
   else 
   {

      while ((wait(&statuschild)) > 0);
      
      FILE *fp,*fp2;
      char path[50];
      fp = popen("ls campur2/*.txt | awk 'BEGIN {FS=\"/\"} {print $2}'","r");
      fp2 = fopen("daftar.txt", "w");

      while (fgets(path, 50, fp))
      {
         fprintf(fp2,"%s\n",path);
      }
      fclose(fp2);
      pclose(fp);

      char *argv[] = {"chmod", "666","daftar.txt", NULL};
      execv("bin/chmod",argv);
   }
}
