#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void usage(char *pname) {
    fprintf(stderr, "USAGE: %s -p <path> [-d depth] [-e extension] [-o]\n", pname);
    exit(EXIT_FAILURE);
}

void scandirectory(const char *path, int depth, const char *ext, const char *outfile) {
    DIR *dirp;
    struct dirent *dp;
    struct stat filestat;
    char true_path[PATH_MAX];
    int fd = -1;

    if (outfile) {
        fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) ERR("open");
    }

    if ((dirp = opendir(path)) == NULL)
        ERR("opendir");

    while ((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        snprintf(true_path, sizeof(true_path), "%s/%s", path, dp->d_name);

        if (lstat(true_path, &filestat))
            ERR("lstat");

        if (S_ISREG(filestat.st_mode)) {
            if (!ext || strstr(dp->d_name, ext)) {
                printf("%s: %ld bytes\n", true_path, filestat.st_size);
                if (fd >= 0) {
                    dprintf(fd, "%s: %ld bytes\n", true_path, filestat.st_size);
                }
            }
        } else if (S_ISDIR(filestat.st_mode) && depth > 1) {
            scandirectory(true_path, depth - 1, ext, outfile);
        }
    }

    closedir(dirp);
    if (fd >= 0) close(fd);
}

int main(int argc, char **argv) {
    int c;
    int depth = 1;
    char *file_ext = NULL;
    int o_flag = 0;
    char *dirs[20];
    int dir_count = 0;
    char *env_path = NULL;

    while ((c = getopt(argc, argv, "p:d:e:o")) != -1) {
        switch (c) {
        case 'p':
            dirs[dir_count++] = optarg;
            break;
        case 'd':
            depth = atoi(optarg);
            break;
        case 'e':
            file_ext = optarg;
            break;
        case 'o':
            o_flag = 1;
            env_path = getenv("L1_OUTPUTFILE");
            if (!env_path || strlen(env_path) == 0)
                ERR("L1_OUTPUTFILE not set");
            break;
        default:
            usage(argv[0]);
        }
    }

    if (dir_count == 0)
        usage(argv[0]);

    for (int i = 0; i < dir_count; i++)
        scandirectory(dirs[i], depth, file_ext, o_flag ? env_path : NULL);

    return EXIT_SUCCESS;
}
