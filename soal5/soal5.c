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
#include <pwd.h>

int main() {
    pid_t pid, sid, child_id;
    int minute = 0;
    int statuschild;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char timestr[50];
    char cmd[500];
    char logcwd[500];

    struct passwd *pw = getpwuid(getuid());
    //To get home directory
    const char *homedir = pw->pw_dir;

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

    sprintf(logcwd, "%s/log",homedir);

    child_id = fork();
    if (child_id == 0) {
        //Creating log dir if not exist
        char *execargv[4] = {"", "-p", logcwd, NULL};
        execv("/bin/mkdir", execargv);
    }
    while ((wait(&statuschild)) > 0);
    kill(child_id, SIGKILL);


    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //Storing daemon pid to /path/to/home/log/.daemonid
    sprintf(cmd, "%s/%s",logcwd, ".daemonid");
    FILE *file = fopen(cmd, "a");
    if (file) {
        fprintf(file, "%d\n",(int) sid);
    }
    fclose(file);
    
    while(1) {
        if (minute%30==0) {
            //Getting time
            t = time(NULL);
            tm = *localtime(&t);
            sprintf(timestr,"%02d:%02d:%04d-%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
        }

        child_id = fork();
        if (child_id==0) {
            //Creating /path/to/home/log/[date] directory if not exist
            sprintf(cmd,"%s/%s", logcwd, timestr);
            char *execargv[4] = {"", "-p", cmd, NULL};
            execv("/bin/mkdir", execargv);
        }
        while ((wait(&statuschild)) > 0);
        kill(child_id, SIGKILL);

        minute++;

        child_id = fork();
        if (child_id==0) {
            //Creating backup of syslog file
            sprintf(cmd,"%s/%s/log%d.log", logcwd, timestr,minute);
            char *execargv[4] = {"", "/var/log/syslog", cmd, NULL};
            execv("/bin/cp", execargv);
        }
        while ((wait(&statuschild)) > 0);
        kill(child_id, SIGKILL);

        sleep(60);
    }

    exit(EXIT_SUCCESS);
}