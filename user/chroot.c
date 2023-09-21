#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if(argc < 3){
    fprintf(2, "Usage: chroot NEWROOT COMMAND [ARGS...]\n");
    exit(1);
  }

  if(chroot(argv[1]) < 0) {
    fprintf(2, "error: chroot()\n");
    exit(1);
  }

  printf("%s is set as the new root dir.\n", argv[1]);
  printf("Exec command...\n");

  if (exec(argv[2], &argv[2])<0) {
      fprintf(2, "error: exec()\n");
      exit(1);
  }

  exit(0);
}