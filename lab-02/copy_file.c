#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Використання: %s <джерело> <призначення>\n", argv[0]);
        return 1;
    }
    int in = open(argv[1], O_RDONLY);
    if (in == -1) {
        fprintf(stderr, "open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    int out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out == -1) {
        fprintf(stderr, "open %s: %s\n", argv[2], strerror(errno));
        close(in);
        return 1;
    }
    char buf[4096];
    ssize_t n;
    while ((n = read(in, buf, sizeof(buf))) > 0) {
        if (write(out, buf, n) == -1) {
            fprintf(stderr, "write: %s\n", strerror(errno));
            close(in);
            close(out);
            return 1;
        }
    }
    if (n == -1) {
        fprintf(stderr, "read: %s\n", strerror(errno));
    }
    close(in);
    close(out);
    return n == -1 ? 1 : 0;
}
