#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "pid_ns.h"
#include "defs.h"

struct pid_ns pid_ns[NPID_NS];

struct spinlock nsid_lock;

int nextnsid = 1;

// init pid namespace table
// initprocを参考にした
void pid_nsinit(void) {
    struct pid_ns *n;
    initlock(&nsid_lock, "nextnsid");

    for (n = pid_ns; n < &pid_ns[NPID_NS]; n++) {
        initlock(&n->lock, "pid_ns");
        n->nsid = 0;
        n->ref = 0;
        n->state = PID_NS_UNUSED;
        n->nextpid = 1;
        n->parent = 0;
        int i;
        for (i = 0; i < NPROCTBL; i++) {
            n->proctbl[i].pid = 0;
            n->proctbl[i].proc = 0;
            n->proctbl[i].state = PID_NS_UNUSED;
        }
    }
}

int allocnsid() {
    int nsid;

    acquire(&nsid_lock);
    nsid = nextnsid;
    nextnsid = nextnsid + 1;
    release(&nsid_lock);

    // printf("new ns id: %d\n", nsid);
    return nsid;
}

struct pid_ns *allocpid_ns(void) {
    struct pid_ns *ns;

    for (ns = pid_ns; ns < &pid_ns[NPID_NS]; ns++) {
        acquire(&ns->lock);
        if (ns->state == PID_NS_UNUSED) {
            goto found;
        } else {
            release(&ns->lock);
        }
    }
    return 0;

found:
    ns->nsid = allocnsid();
    ns->state = PID_NS_USED;

    release(&ns->lock);

    initlock(&ns->pid_lock, "nextpid");

    return ns;
}


void freepid_ns(struct pid_ns *ns) {
    // printf("free ns: %d\n", ns->nsid);

    ns->nsid = 0;
    ns->ref = 0;
    ns->state = PID_NS_UNUSED;
    ns->nextpid = 1;
    ns->parent = 0;
    int i;
    for (i = 0; i < NPROCTBL; i++) {
        ns->proctbl[i].pid = 0;
        ns->proctbl[i].proc = 0;
        ns->proctbl[i].state = PID_NS_UNUSED;
    }

}

int allocnspid(struct pid_ns *ns) {
  int pid;
  
  acquire(&ns->pid_lock);
  pid = ns->nextpid;
  ns->nextpid = ns->nextpid + 1;
  ns->ref = ns->ref + 1;
  release(&ns->pid_lock);

  return pid;
}



struct proctable *allocproctbl(struct pid_ns *ns) {
    struct proctable *tbl;

    for (tbl = ns->proctbl; tbl < &ns->proctbl[NPROCTBL]; tbl++) {
        acquire(&ns->lock);
        if (tbl->state == PID_NS_UNUSED) {
            goto found;
        } else {
            release(&ns->lock);
        }
    }
    return 0;

found:
    tbl->state = PID_NS_USED;
    release(&ns->lock);

    return tbl;
}

void freeproctbl(struct pid_ns *ns, struct proc *proc) {
    struct proctable *tbl;

    for (tbl = ns->proctbl; tbl < &ns->proctbl[NPROCTBL]; tbl++) {
        acquire(&ns->lock);
        if (tbl->proc == proc) {
            goto found;
        } else {
            release(&ns->lock);
        }
    }
    return;

found:
    tbl->pid = 0;
    tbl->proc = 0;
    tbl->state = PID_NS_UNUSED;
    ns->ref -= 1;

    release(&ns->lock);

    // printf("ns: %d, ref: %d\n", ns->nsid, ns->ref);

    struct pid_ns *parent = ns->parent;

    if (ns->ref <= 0) {
        freepid_ns(ns);
    }

    if (parent > 0 && parent->state == PID_NS_USED) {
        freeproctbl(parent, proc);
    }

    return;
}
