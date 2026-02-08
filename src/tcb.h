#ifndef TCB_H
#define TCB_H

#include "context.h"

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SUSPENDED
} task_state_t;

typedef struct tcb {
    struct context ctx;

    unsigned int id;
    task_state_t state;

    unsigned char *stack_base;   // lowest address
    unsigned int stack_size;

    unsigned int stack_high_water; // bytes used

    struct tcb *next;
} tcb_t;


#endif
