#include <sys/wait.h>
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

int main() {
    pid_t pid, sid;
    int minute = 0;
    int statuschild;
    pid_t child_id;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char timestr[50];
    char cmd[500];
    char minutestr[100];

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

    if (fork() == 0) {
        sprintf(cmd, "mkdir -p ~/log/ && echo %d > ~/log/.daemonid",(int)sid);
        char *execargv[4] = {"", "-c", cmd, NULL};
        execv("/bin/bash", execargv);
    }
    
    

    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while(1) {
        if (minute%30==0) {
            t = time(NULL);
            tm = *localtime(&t);
            sprintf(timestr,"%02d:%02d:%04d-%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
        }

        sprintf(cmd,"mkdir -p ~/log/ && cd ~/log/ && folder=\"%s\" && mkdir -p \"$folder\" && cat /var/log/syslog > ~/log/$folder/log", timestr);
        sprintf(minutestr, "%d", ++minute);
        strcat(cmd, minutestr);
        strcat(cmd, ".log");
        
        child_id = fork();
        if (child_id==0) {
            char *execargv[4] = {"", "-c", cmd, NULL};
            execv("/bin/bash", execargv);
        }
        else
        {
            while ((wait(&statuschild)) > 0);
            // prevent from creating zombie proccess
            kill(child_id, SIGKILL);
            sleep(60);
        }
    }

    exit(EXIT_SUCCESS);
}