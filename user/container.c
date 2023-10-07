#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "kernel/namespace.h"
#include "user/user.h"

#define TEST_DIR "/container"

int container(char* dir, char **argv) {
    // Create a test directory
    // if (mkdir(dir) == -1) {
    //     fprintf(2, "Failed to create test directory\n");
    //     return 0;
    // }

    // Change root to the test directory
    if (chroot(dir) == -1) {
        fprintf(2, "Failed to change root directory\n");
        return 0;
    }

    // Change current working directory to the new root directory
    if (chdir("/") == -1) {
        fprintf(2, "Failed to change current working directory\n");
        return 0;
    }

    

    exec(argv[0], argv);

    return 1;
}

int main(int argc, char* argv[]) {
    
    if(argc != 3){
        fprintf(2, "Usage: container NEWROOT COMMAND [ARGS...]\n");
        exit(1);
    }

    if (unshare(NEW_PID_NS) < 0 ) {
        fprintf(2, "Failed to unshare\n");
        return 1;
    }

    int pid = fork();
    if (pid == -1) {
        fprintf(2, "Failed to fork\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        if (container(argv[1], &argv[2])) {
            return 0;
        } else {
            return 1;
        }
    } else {
        // Parent process
        wait(&pid);
    }
    return 0;
}