#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "capability.h"


uint64
sys_unshare(void)
{
  if (!checkcap(myproc(), CAP_SYS_UNSHARE)) {
      return -1;
  }

  int flag;

  argint(0, &flag);
  return unshare(flag);
}
