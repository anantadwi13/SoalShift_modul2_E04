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

  exit(EXIT_SUCCESS);
}
