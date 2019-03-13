#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
#include<stdio.h>
#include <time.h>

int main(int argc, char** argv) {
    int minute = 0;
    int statuschild;
    pid_t child_id;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char timestr[50];
    char cmd[150];
    char cmdWithMinute[500];
    char minutestr[100];

    //char *cmd = "mkdir -p ~/log/ && cd ~/log/ && date +%d:%m:%Y-%H:%M | xargs mkdir -p && cat /var/log/syslog > ~/log/";
    //char cmd[150] = "mkdir -p ~/log/ && cd ~/log/ && folder=`date +%d:%m:%Y-%H:%M` && mkdir -p \"$folder\" && cat /var/log/syslog > ~/log/$folder/log";

    while(1){
        if (minute%30==0) {
            t = time(NULL);
            tm = *localtime(&t);
            sprintf(timestr,"%02d:%02d:%04d-%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
        }
        sprintf(cmd,"mkdir -p ~/log/ && cd ~/log/ && folder=\"%s\" && mkdir -p \"$folder\" && cat /var/log/syslog > ~/log/$folder/log", timestr);
        sprintf(minutestr, "%d", ++minute);
        strcpy(cmdWithMinute, cmd);
        strcat(cmdWithMinute, minutestr);
        strcat(cmdWithMinute, ".log");
        //printf("%s\n", cmdWithMinute);
        
        child_id = fork();
        if (child_id==0) {
            char *execargv[4] = {"", "-c", cmdWithMinute, NULL};
            execv("/bin/bash", execargv);
        }
        else
        {
            while ((wait(&statuschild)) > 0);
            // prevent zombie proccess
            kill(child_id, SIGKILL);
            sleep(1);
        }
    }
}