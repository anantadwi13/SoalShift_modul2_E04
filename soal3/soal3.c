#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>

int main(int argc, char** argv) {
    pid_t child_id;
    pid_t grandchild_id;
    int statuschild;
    int statusgrandchild;

    child_id = fork();

    if (child_id == 0) {
        grandchild_id = fork();

        if (grandchild_id == 0) {
            char *execargv[4] = {"", "-oq", argv[1], NULL};
            execv("/usr/bin/unzip", execargv);
        }
        else
        {
            while ((wait(&statusgrandchild)) > 0);
            char *execargv[4] = {"","-c", "ls campur2/*.txt | awk 'BEGIN {FS=\"/\"} {print $2}' > daftar.txt", NULL};
            execv("/bin/bash", execargv);
        }
    } else {
        while ((wait(&statuschild)) > 0);

        char *execargv[4] = {"","-c", "chmod 666 daftar.txt", NULL};
        execv("/bin/bash", execargv);
    }
}