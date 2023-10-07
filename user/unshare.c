#include "kernel/types.h"
#include "user/user.h"
#include "kernel/namespace.h"

int main(int argc, char *argv[]) {
  if(argc != 1){
    fprintf(2, "Usage: unshare\n");
    exit(1);
  }

  if(unshare(NEW_PID_NS) < 0) {
    fprintf(2, "error: unshare()\n");
    exit(1);
  }

  exit(0);
}