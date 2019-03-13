#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h> 
#include <stdio.h> 

int main(int argc, char **args) {
    DIR *d;
    struct dirent *dir;
    int size;
    char namaGambar[500];
    char namaGambarSource[500];

    if (argc < 3) {
        printf("./cmdfile /path/images/from /path/images/to\n");
        return 0;
    }

    if (fork() == 0) {
        char *argv[4] = {"", "-p", args[2], NULL};
        execv("/bin/mkdir", argv);
    }

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
        d = opendir(args[1]);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                size = strlen(dir->d_name);
                char *namafile = dir->d_name;
                if ((namafile[size-1] == 'g' || namafile[size-1] == 'G') && (namafile[size-2] == 'n' || namafile[size-2] == 'N') && (namafile[size-3] == 'p' || namafile[size-3] == 'P') && namafile[size-4] == '.') {
                    strcpy(namaGambar, args[2]);
                    strcat(namaGambar, namafile);
                    namaGambar[strlen(namaGambar)-4] = '\0';
                    strcat(namaGambar, "_grey.png");
                    strcpy(namaGambarSource, args[1]);
                    strcat(namaGambarSource, namafile);

                    if (fork() == 0) {
                        char *argv[4] = {"", namaGambarSource, namaGambar, NULL};
                        execv("/bin/cp", argv);
                    }
                }
            }
            closedir(d);
        }
        sleep(10);
    }

    exit(EXIT_SUCCESS);
}