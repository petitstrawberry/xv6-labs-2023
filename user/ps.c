#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
  if(argc != 1){
    fprintf(2, "Usage: ps\n");
    exit(1);
  }

  if (getproclist() < 0) {
      fprintf(2, "error: getproclist\n");
  }

  exit(0);
}