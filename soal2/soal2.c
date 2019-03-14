#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main() {
    pid_t pid, sid;
  
    struct dirent  *dp;
    struct stat     statbuf;
    char user[100];
    char group[100];
    char *file = "/home/anantadwi13/SISOP/modul2/shift/soal2/hatiku/elen.ku";

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
        if (access(file, F_OK) != -1) {
            stat(file, &statbuf);
            sprintf(user, "%s",getpwuid(statbuf.st_uid)->pw_name);
            sprintf(group, "%s",getgrgid(statbuf.st_gid)->gr_name);
            if (strcmp(user, "www-data") == 0 && strcmp(group, "www-data") == 0) {
                remove(file);
            }
        }
        sleep(3);
    }
    
    exit(EXIT_SUCCESS);
}