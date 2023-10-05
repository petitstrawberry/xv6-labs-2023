#include "namespace.h"

struct namespace namespace[NNAMESPACE];

struct spinlock nsid_lock;

int nextnsid = 1;

// init namespace table
// initprocを参考にした
void initnamespace(void) {
    struct namespace *n;
    initlock(&nsid_lock, "nextnsid");

    for (n = namespace; n < &namespace[NNAMESPACE]; n++) {
        initlock(&n->lock, "namespace");
        n->state = UNUSED;
    }
}