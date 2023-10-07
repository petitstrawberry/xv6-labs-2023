
enum pid_ns_state { PID_NS_UNUSED, PID_NS_USED };

struct proctable {
    int pid;            // pid in the namespace
    struct proc* proc;
    enum pid_ns_state state;
};

struct pid_ns {
    struct spinlock lock;

    int nsid;
    enum pid_ns_state state;
    int nextpid;

    struct pid_ns *parent;

    struct spinlock pid_lock;
    struct proctable proctbl[NPROCTBL];  // proctable head

    // Not need lock
    struct pid_ns *child;  // child namespace
    struct pid_ns *next;   // next namespace in same level
};
