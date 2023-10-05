#include "defs.h"
#include "memlayout.h"
#include "param.h"
#include "proc.h"
#include "riscv.h"
#include "spinlock.h"
#include "types.h"

#define NNAMESPACE 64

enum namespacestate { UNUSED, USED };

struct namespace {
    struct spinlock lock;
    enum namespacestate state;

    struct namespace *child;  // child namespace
    struct namespace *next;   // next namespace in same level

    struct proc proc[NPROC];  // Processes
    char name[16];
};
