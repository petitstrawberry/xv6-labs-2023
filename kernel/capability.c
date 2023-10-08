#include "types.h"
#include "riscv.h"
#include "spinlock.h"
#include "param.h"
#include "proc.h"
#include "defs.h"
#include "capability.h"

int capsetp(struct proc *proc, int caps) {
    int mask = proc->parent->caps;
    proc->caps = mask & caps;
    return proc->caps;
}

int checkcap(struct proc *proc, int cap) {
    return (proc->caps & cap) == cap;
}