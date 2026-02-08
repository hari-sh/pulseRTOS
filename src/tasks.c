#include "tasks.h"
#include "context.h"
#include "uart.h"
#include "timer.h"
#include "tcb.h"
#include "mem_pool.h"

extern mem_pool_t tcb_pool;

// ALIGNED STACKS to avoid 'T'-only trap loops
stack_t task_stacks[MAX_TASKS];
unsigned char stacks[MAX_TASKS][STACK_SIZE];
static tcb_t *task_list[MAX_TASKS];
static int current_task = -1;
static int task_count = 0;

int num_active_tasks = MAX_TASKS;
int are_tasks_initialized = 0;

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
    tcb_t *t = (tcb_t *)mem_alloc(&tcb_pool);
    if (!t)
        return;

    t->id = id;
    t->state = TASK_READY;

    t->stack_base = stacks[id];
    t->stack_size = STACK_SIZE;

    t->ctx.ra = (unsigned int)task_entry;
    t->ctx.sp = (unsigned int)(stacks[id] + STACK_SIZE);
    t->ctx.a0 = (unsigned int)fn;
    t->ctx.mepc = (unsigned int)task_entry;

    task_list[id] = t;
    task_count++;
}

void schedule(void)
{
    int prev = current_task;
    current_task++;

    if (current_task >= task_count)
        current_task = 0;

    uart_puts_2_int("%d -> %d\n", prev, current_task);

    if (prev == -1) {
        struct context dummy;
        context_switch(&dummy, &task_list[0]->ctx);
    } else {
        context_switch(&task_list[prev]->ctx, &task_list[current_task]->ctx);
    }
}

void taskA(void) {
    while(1);
}

void taskB(void) {
    while(1);
}

void taskC(void) {
    while(1);
}

void setTaskInit()  {
    are_tasks_initialized = 1;
}

int getTaskInit()  {
    return are_tasks_initialized;
}