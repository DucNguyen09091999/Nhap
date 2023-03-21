#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    DIR *dp;
    struct stat file_stats;
    struct dirent *ep;
    int ret = EXIT_SUCCESS;
    int ret2;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s FILE...\n", argv[0]);
        return EXIT_FAILURE;
    }

    dp = opendir(argv[1]);
    if (dp == NULL) {
        perror("Couldn't open directory");
        return EXIT_FAILURE;
    }

    ret2 = chdir(argv[1]);
    if (ret2 == -1) {
        perror("Unable to change directory");
        ret = EXIT_FAILURE;
        goto out1;
    }

    while ((ep = readdir(dp))) {
        printf("filename: %s\n", ep->d_name);

        if ((stat(ep->d_name, &file_stats)) == -1) {
            perror("fstat");
            ret = EXIT_FAILURE;
            goto out2;
        }

        printf(" device: %ld\n",
                file_stats.st_dev);
        printf(" protection: %o\n",
                file_stats.st_mode);
        printf(" number of hard links: %ld\n",
                file_stats.st_nlink);
        printf(" user ID of owner: %d\n",
                file_stats.st_uid);
        printf(" group ID of owner: %d\n",
                file_stats.st_gid);
        printf(" device type (if inode device): %ld\n",
                file_stats.st_rdev);
        printf(" total size, in bytes: %ld\n",
                file_stats.st_size);
        printf(" blocksize for filesystem I/O: %ld\n",
                file_stats.st_blksize);
        printf(" inode number: %lu\n",
                file_stats.st_ino);
        printf(" time of last access: %ld : %s",
                file_stats.st_atime,
                ctime(&file_stats.st_atime));
        printf(" time of last change: %ld : %s\n",
                file_stats.st_ctime,
                ctime(&file_stats.st_ctime));
    }

out2:
    ret2 = chdir("..");
    if (ret2 == -1) {
        perror("Unable to change directory back");
        ret = EXIT_FAILURE;
        goto out1;
    }

out1:
    closedir(dp);
    return ret;
}