#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int fds[2];
    // i bytes buffer
    char buf = 0;

    // make pipe
    if (pipe(fds) < 0) {
        fprintf(2,"error: pipe()\n");
        exit(1);
    }

    // fork
    int pid = fork();

    if (pid < 0) {
        fprintf(2, "error: fork()\n");
        exit(1);
    }

    if (pid == 0) {
        // child

        if(read(fds[0], &buf, sizeof(buf)) < 0) {
            fprintf(2, "read()");
            exit(1);
        }

        printf("%d: received ping\n", getpid());

        if(write(fds[1], &buf, sizeof(buf)) < 0) {
            fprintf(2, "error: write()");
            exit(1);
        }

        // close pipe
        close(fds[0]);
        close(fds[1]);
    } else {
        buf = 1;
        if(write(fds[1], &buf, sizeof(buf)) < 0) {
            fprintf(2, "error: write()");
            exit(1);
        }

        // wait for child
        wait(&pid);

        if(read(fds[0], &buf, sizeof(buf)) < 0) {
            fprintf(2, "error: read()");
            exit(1);
        }

        printf("%d: received pong\n", getpid());


        // close pipe
        close(fds[0]);
        close(fds[1]);

    }

  exit(0);
}
