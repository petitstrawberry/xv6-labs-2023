#define NPID_NS 64

enum pid_ns_state { PID_NS_UNUSED, PID_NS_USED };

struct pid_ns {
    struct spinlock lock;

    int nsid;
    enum pid_ns_state state;
    int nextpid;
    struct spinlock pid_lock;

    // Not need lock
    struct pid_ns *child;  // child namespace
    struct pid_ns *next;   // next namespace in same level
};
