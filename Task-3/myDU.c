#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int get_sum_of_files_and_root_dir(char *dir) {
    DIR *d = opendir(dir);
    struct dirent *e;
    struct stat s;
    int t = 0;

    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name, "..")) continue;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, e->d_name);
        if (stat(path, &s) == -1) continue;
        if (S_ISDIR(s.st_mode) || S_ISREG(s.st_mode)) t += s.st_size;
    }

    closedir(d);
    return t;
}

int get_recursive_sum_of_files_in_dir(char *dir, int w, int md, int cd) {
    DIR *d = opendir(dir);
    struct dirent *e;
    struct stat s;
    int t = 0;
    char b[1024];
    memset(b, '\0', 1024);

    if ((cd == 0) && (!d)) { perror("Error opening directory"); return 0; }

    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        char p[1024];
        snprintf(p, sizeof(p), "%s/%s", dir, e->d_name);
        lstat(p, &s);

        switch (s.st_mode & S_IFMT) {
            case S_IFDIR:
                t += s.st_size;
                t += get_recursive_sum_of_files_in_dir(p, w, md, cd + 1);
                break;
            case S_IFLNK: {
                char sp[1024], nsp[1024], fsp[1024];
                snprintf(sp, strlen(p) - strlen(e->d_name), "%s", p);
                int n = readlink(p, nsp, 1023);
                if (n == -1) { perror("readlink"); exit(EXIT_FAILURE); }
                snprintf(fsp, strlen(sp) + 1 + n, "%s/%s", sp, nsp);
                t += get_recursive_sum_of_files_in_dir(fsp, w, md, cd + 1);
                break;
            }
            case S_IFREG:
                t += s.st_size;
                break;
            default:
                break;
        }
    }

    closedir(d);
    if (cd == 0) {
        sprintf(b, "%d", t);
        write(w, b, strlen(b));
        return 0;
    } else {
        return t;
    }
}

int main(int ac, char *av[]) {
    int ts = 0;
    char id[1024];
    struct dirent *ep;
    struct stat es;

    memset(id, '\0', 1023);
    strcpy(id, av[1]);
    DIR *dp = opendir(id);

    if (!dp) { perror("Not a directory, provide directory as input"); exit(-1); }

    while ((ep = readdir(dp)) != NULL) {
        if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, "..")) continue;
        if (ep->d_type == DT_DIR || ep->d_type == DT_LNK) {
            int p[2];
            if (pipe(p) == -1) { perror("Pipe creation failed"); exit(EXIT_FAILURE); }
            pid_t r = fork();
            if (r == 0) {
                char pp[1024];
                close(p[0]);
                snprintf(pp, strlen(id) + 1 + strlen(ep->d_name) + 1, "%s/%s", id, ep->d_name);
                get_recursive_sum_of_files_in_dir(pp, p[1], 0, 0);
                close(p[1]);
                exit(0);
            } else {
                close(p[1]);
                char sb[1024];
                memset(sb, '\0', 1023);
                ssize_t br = read(p[0], sb, sizeof(sb));
                if (br == -1) { perror("error reading size from pipe"); exit(EXIT_FAILURE); }
                sb[br] = '\0';
                int rd = atoi(sb);
                ts += rd;
                waitpid(r, NULL, 0);
                close(p[0]);
            }
        }
    }

    int rs = get_sum_of_files_and_root_dir(id);
    ts += rs;
    printf("%d\n", ts);
    closedir(dp);

    return 0;
}
