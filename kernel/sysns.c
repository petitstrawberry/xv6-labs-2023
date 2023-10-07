#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"


uint64
sys_unshare(void)
{
  int flag;

  argint(0, &flag);
  return unshare(flag);
}
