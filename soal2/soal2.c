// #include <sys/types.h>
// #include <sys/stat.h>
// #include <dirent.h>
// #include <pwd.h>
// #include <grp.h>
// #include <time.h>
// #include <locale.h>
// #include <langinfo.h>
// #include <stdio.h>
// #include <stdint.h>


// int main(){
//     struct dirent  *dp;
//     struct stat     statbuf;
//     struct passwd  *pwd;
//     struct group   *grp;
//     struct tm      *tm;
//     char            datestring[256];
//     DIR* dir = opendir(".");
//     while ((dp = readdir(dir)) != NULL) {


//     /* Get entry's information. */
//     if (stat(dp->d_name, &statbuf) == -1)
//         continue;


//     // /* Print out type, permissions, and number of links. */
//     // printf("%10.10s", sperm (statbuf.st_mode));
//     // printf("%4d", statbuf.st_nlink);


//     /* Print out owner's name if it is found using getpwuid(). */
//     if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
//         printf(" %-8.8s", pwd->pw_name);
//     else
//         printf(" %-8d", statbuf.st_uid);


//     /* Print out group name if it is found using getgrgid(). */
//     if ((grp = getgrgid(statbuf.st_gid)) != NULL)
//         printf(" %-8.8s", grp->gr_name);
//     else
//         printf(" %-8d", statbuf.st_gid);


//     /* Print size of file. */
//     printf(" %9jd", (intmax_t)statbuf.st_size);


//     tm = localtime(&statbuf.st_mtime);


//     /* Get localized date string. */
//     strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);


//     printf(" %s %s\n", datestring, dp->d_name);
// }
// }

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    uid_t uid;
    struct passwd *pwd;
    char *endptr;

    if (argc != 3 || argv[1][0] == '\0') {
        fprintf(stderr, "%s <owner> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uid = strtol(argv[1], &endptr, 10);  /* Allow a numeric string */

    if (*endptr != '\0') {         /* Was not pure numeric string */
        pwd = getpwnam(argv[1]);   /* Try getting UID for username */
        if (pwd == NULL) {
            perror("getpwnam");
            exit(EXIT_FAILURE);
        }

        uid = pwd->pw_uid;
    }

    if (chown(argv[2], uid, -1) == -1) {
        perror("chown");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}