#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int counter = 0;

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    char temp1[12], temp2[12];
    int dif1, dif2, dif3;
    struct stat details;

    time_t t = time(NULL);
    char *waktu1 = ctime(&t);
	//printf("%s", waktu1);

    int i;
    for (i=0; i<=11; i++)
    {
       if (i==11) temp1[i]='\0';
       else temp1[i] = waktu1[strlen(waktu1)-17+i];
    }
	/*
	for (i=0; i<11 ; i++)
	{
	   printf("%c",temp1[i]);
	}
	*/

    stat("/home/anantadwi13/Documents/makanan/makan_enak.txt", &details);

    char *waktu2 = ctime(&details.st_atime);
	//printf("\n%s", waktu2);

    for (i=0; i<=11; i++)
    {
       if (i==11) temp2[i]='\0';
       else temp2[i] = waktu1[strlen(waktu2)-17+i];
    }
	/*
	for (i=0; i<11 ; i++)
	{
	   printf("%c",temp2[i]);
	}
	*/

    int daydif;
    daydif = ((temp1[0]-'0')*10) + (temp1[1]-'0') - ((temp2[0]-'0')*10) - (temp2[1]-'0');
	//printf("\n%d\n",daydif);

    dif1=((temp1[3]-'0')*10) + (temp1[4]-'0') - ((temp2[3]-'0')*10) - (temp2[4]-'0');
    dif2=((temp1[6]-'0')*10) + (temp1[7]-'0') - ((temp2[6]-'0')*10) - (temp2[7]-'0');
    dif3=((temp1[9]-'0')*10) + (temp1[10]-'0') - ((temp2[9]-'0')*10) - (temp2[10]-'0');
	//printf("%d %d %d",dif1, dif2, dif3);
    int second = dif1*3600 + dif2*60 + dif3;
    if (daydif == 0 && second <= 30)
    {
      char temp3[200];
      counter++;
      sprintf(temp3,"/home/anantadwi13/Documents/makanan/makan_sehat%d.txt",counter);
      FILE *fPoint;
      fPoint = fopen(temp3, "a");
      fclose(fPoint);
    }
    sleep(5);
  }
  
  exit(EXIT_SUCCESS);
}
