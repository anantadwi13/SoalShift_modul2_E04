#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
#include<stdio.h>
#include <time.h>

int main(int argc, char** argv) {
    char *execargv[4] = {"", "-c", "if [ -e ~/log/.daemonid ]; then cat ~/log/.daemonid | xargs kill > /dev/null 2>&1; rm ~/log/.daemonid; fi", NULL};
    execv("/bin/bash", execargv);
}