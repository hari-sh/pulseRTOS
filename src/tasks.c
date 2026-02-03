#include "tasks.h"
#include "context.h"
#include "uart.h"
#include "timer.h"

// ALIGNED STACKS to avoid 'T'-only trap loops
unsigned char stacks[MAX_TASKS][STACK_SIZE] __attribute__((aligned(16)));
struct tcb tasks[MAX_TASKS];

int current_task = -1;
int num_active_tasks = MAX_TASKS;

void task_entry(void (*fn)(void))
{
    // Enable interrupts in mstatus so this task can be preempted!
    asm volatile("csrs mstatus, %0" : : "r" (1 << 3)); // MIE
    
    fn();
    
    // Fallback if task returns (should not happen in this demo)
    while(1);
}

void init_task(int id, void (*fn)(void))
{
    struct tcb *t = &tasks[id];

    t->state = TASK_READY;

    if (id == 0) t->priority = 1;   // Task A
    if (id == 1) t->priority = 0;   // Task B (highest)
    if (id == 2) t->priority = 2;   // Task C

    t->ctx.ra   = (unsigned int)task_entry;
    t->ctx.sp   = (unsigned int)(stacks[id] + STACK_SIZE);
    t->ctx.a0   = (unsigned int)fn;
    t->ctx.mepc = (unsigned int)task_entry;
}

void schedule(void)
{
    int prev = current_task;
    int best = -1;

    /* Find highest-priority runnable task */
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].state == TASK_BLOCKED)
            continue;

        if (best == -1 ||
            tasks[i].priority < tasks[best].priority) {
            best = i;
        }
    }

    if (best == -1)
        return;

    if (best == current_task)
        return;

    if (prev >= 0) {
        tasks[prev].state = TASK_READY;
        tasks[best].state = TASK_RUNNING;
        current_task = best;

        context_switch(&tasks[prev].ctx,
                       &tasks[best].ctx);
    }
}


void start_scheduler(void)
{
    current_task = 0;
    tasks[0].state = TASK_RUNNING;

    struct context dummy;
    context_switch(&dummy, &tasks[0].ctx);
}


void taskA(void) {
    while(1) {
        uart_putc('A');
        for (volatile int i = 0; i < 50000000; i++); 
    }
}

void taskB(void) {
    while(1) {
        uart_putc('B');
        for (volatile int i = 0; i < 50000000; i++);
    }
}

void taskC(void) {
    while(1) {
        uart_putc('C');
        for (volatile int i = 0; i < 50000000; i++);
    }
}
