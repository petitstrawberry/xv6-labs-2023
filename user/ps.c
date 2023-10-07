#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#include "user/process.h"

int main(int argc, char *argv[]) {
  if(argc != 1){
    fprintf(2, "Usage: ps\n");
    exit(1);
  }

  struct process procs[NPROCTBL];

  if (getproclist() < 0) {
      fprintf(2, "error: getproclist\n");
  }

  for (int i = 0; i < sizeof(procs); i++)
  {
    //   printf("pid: %d, name: %s\n", procs[i].pid, procs[i].name);
  }

  exit(0);
}