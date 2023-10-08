#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "capability.h"

uint64 sys_capsetp(void) {
    if (!checkcap(myproc(), CAP_SYS_CAPSETP)) {
      return -1;
    }

    int pid;
    int flags;

    argint(0, &pid);
    argint(1, &flags);

    struct proc *proc = getproc(pid);

    return capsetp(proc, flags);
}