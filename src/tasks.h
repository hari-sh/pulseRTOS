#ifndef TASKS_H
#define TASKS_H
#include "context.h"

#define MAX_TASKS 3
#define STACK_SIZE 1024

void init_task(int id, void (*fn)(void));
void start_scheduler(void);
void schedule(void);

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED
} task_state_t;


struct tcb {
    struct context ctx;     // MUST be first (important later)
    task_state_t state;
    int priority;           // not used yet, but added now
};


// Task entry wrapper
void task_entry(void (*fn)(void));

// Example tasks
void taskA(void);
void taskB(void);
void taskC(void);

#endif
