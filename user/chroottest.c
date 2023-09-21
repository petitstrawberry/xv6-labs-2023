#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "user/user.h"

#define TEST_DIR "/chroot_test"
#define TEST_FILE "/you_are_in_jail"

int test_chroot() {
    // Create a test directory
    if (mkdir(TEST_DIR) == -1) {
        fprintf(2, "Failed to create test directory\n");
        return 0;
    }

    // Create a new file TEST_FILE in the TEST_DIR directory
    int fd = open(TEST_DIR TEST_FILE, O_CREATE | O_WRONLY);
    if (fd == -1) {
        fprintf(2, "Failed to create file\n");
        return 0;
    }
    // Close the file
    if (close(fd) == -1) {
        fprintf(2, "Failed to close file\n");
        return 0;
    }

    // Change root to the test directory
    if (chroot(TEST_DIR) == -1) {
        fprintf(2, "Failed to change root directory\n");
        return 0;
    }


    struct stat st;

    // Change current working directory to the new root directory
    if (chdir("/") == -1) {
        fprintf(2, "Failed to change current working directory\n");
        return 0;
    }

    // Check if the file exists
    if (stat(TEST_FILE, &st) == -1) {
        fprintf(2, "Failed to stat file\n");
        fprintf(2, "chroot() may not be working.\n");
        return 0;
    }

    // Check if the program can escape from the TEST_DIR directory
    if (chdir("../") == -1) {
        fprintf(2, "Failed to change current working directory\n");
        return 0;
    }
    // Check if the file exists
    if (stat(TEST_FILE, &st) == -1) {
        fprintf(2, "Failed to stat file\n");
        fprintf(2, "chroot() may not be working.\n");
        fprintf(2, "Process can escape from the new root directory.\n");
        return 0;
    }

    // Check if the program can escape from the TEST_DIR directory
    if (chdir("../../") == -1) {
        fprintf(2, "Failed to change current working directory\n");
        return 0;
    }
    // Check if the file exists
    if (stat(TEST_FILE, &st) == -1) {
        fprintf(2, "Failed to stat file\n");
        fprintf(2, "chroot() may not be working.\n");
        fprintf(2, "Process can escape from the new root directory.\n");
        return 0;
    }

    return 1;
}

int main() {
    // If the root directory is changed by chroot(),
    // the test directory cannot be cleaned up,
    // so fork() and run chroot() in a separate process.
    int pid = fork();
    if (pid == -1) {
        fprintf(2, "Failed to fork\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        if (test_chroot()) {
            printf("chroot test passed\n");
            return 0;
        } else {
            printf("chroot test failed\n");
            return 1;
        }
    } else {
        // Parent process
        wait(&pid);
        // Cleanup
        if (unlink(TEST_DIR TEST_FILE) == -1) {
            fprintf(2, "Failed to remove test file\n");
            return 1;
        }
        if (unlink(TEST_DIR) == -1) {
            fprintf(2, "Failed to remove test directory\n");
            return 1;
        }
    }
    return 0;
}