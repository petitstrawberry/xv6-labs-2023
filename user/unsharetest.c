#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/namespace.h"

#define TEST_DIR "/container_test"
#define TEST_FILE "/you_are_in_jail"

int test_unshare() {
    
    if (unshare(NEW_PID_NS)<0) {
        fprintf(2, "Failed to unshare pid namespace\n");
        return 1;
    }

    int pid1 = fork();
    
    if (pid1 == -1) {
        fprintf(2, "Failed to fork\n");
        return 1;
    } else if (pid1 == 0) {

        printf("[ns2] child pid1: %d\n", getpid());
        if (getpid() != 1) {
            return 1;
        }

        if (unshare(NEW_PID_NS)<0) {
            fprintf(2, "Failed to unshare pid namespace\n");
            return 0;
        }

        int pid2 = fork();
        if (pid2 == -1) {
          fprintf(2, "Failed to fork\n");
            return 1;
        } else if (pid2 == 0) {
            printf("[ns3] child pid1: %d\n", getpid());
            if (getpid() != 1) {
                return 1;
            }
            sleep(10);
            exit(0);
        }
        wait(&pid2);
        exit(0);
    } else {
        printf("[ns1] parent pid1: %d\n", getpid());

        int pid2 = fork();
        if (pid2 == -1) {
        fprintf(2, "Failed to fork\n");
            return 1;
        } else if (pid2 == 0) {
            printf("[ns2] child pid2: %d\n", getpid());
            if (getpid() != 2) {
                return 1;
            }

            sleep(50);
            exit(0);
        }
    
        // Parent process
        printf("wait for %d\n", pid1);
        printf("wait for %d\n", pid2);
        wait(&pid1);
        wait(&pid2);
    }

    return 0;
}

int main() {
    // If the root directory is changed by chroot(),
    // the test directory cannot be cleaned up,
    // so fork() and run chroot() in a separate process.
    
    // Child process
    if (!test_unshare()) {
        printf("unshare test passed\n");
        return 0;
    } else {
        printf("unshare test failed\n");
        return 1;
    }

}