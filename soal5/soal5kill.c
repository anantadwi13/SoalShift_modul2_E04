#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
#include<stdio.h>
#include <time.h>
#include <pwd.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    char cmd[500];
    char pid[100];
    
    struct passwd *pw = getpwuid(getuid());

    const char *homedir = pw->pw_dir;

    sprintf(cmd, "%s/log/%s",homedir, ".daemonid");
    FILE *file = fopen(cmd, "r");
    if (file) {
        while(fgets(pid, sizeof(pid), file)){
            printf("%s\n",pid);
            kill(atoi(pid), SIGKILL);
        }
    }
    fclose(file);
    file = fopen(cmd, "w");
    fclose(file);
}