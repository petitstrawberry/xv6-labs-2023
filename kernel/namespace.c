#include "types.h"
#include "param.h"
// #include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "namespace.h"
#include "defs.h"
#include "pid_ns.h"


int unshare(int flag) {
    struct proc *proc = myproc();
    if (flag & NEW_PID_NS) {
        struct pid_ns *ns = allocpid_ns();

        if (ns) {
            ns->parent = proc->ns;
            proc->child_pid_ns = ns;
        }else {
            return -1;
        }
    }
    return 0;
}