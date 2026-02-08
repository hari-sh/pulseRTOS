#include "tasks.h"
#include "context.h"
#include "uart.h"
#include "timer.h"

// ALIGNED STACKS to avoid 'T'-only trap loops
unsigned char stacks[MAX_TASKS][STACK_SIZE] __attribute__((aligned(16)));
struct context tasks[MAX_TASKS];

int current_task = -1;
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
    // Point ra to task_entry wrapper
    tasks[id].ra = (unsigned int)task_entry;
    
    // sp points to top of stack
    tasks[id].sp = (unsigned int)(stacks[id] + STACK_SIZE);
    
    // a0 gets the function pointer (argument to task_entry)
    tasks[id].a0 = (unsigned int)fn;
    
    // mepc also points to entry (used by mret if we strictly used mret, 
    // but context_switch uses ret. However, if we switch TO it via interrupt 
    // path, we might rely on proper mepc. 
    // actually our context_switch uses ret, so ra is used.
    // mepc is only used if we switch via mret which we don't do directly in context_switch.)
    // Wait, context_switch loads t0 from mepc slot and writes to mepc CSR.
    // But it returns via `ret` (using ra).
    // The trap handler does `mret`.
    // If context_switch is called from trap_handler:
    //   It returns to trap_handler.
    //   trap_handler does mret using CURRENT mepc CSR.
    //   So context_switch MUST restore mepc CSR.
    // Correct.
    tasks[id].mepc = (unsigned int)task_entry;
}

void schedule(void)
{
    int prev = current_task;
    current_task++;
    if (current_task >= MAX_TASKS)  {
        current_task = 0;
    }
    uart_puts_2_int("%d -> %d\n", prev, current_task);
    if (prev == -1) {
        struct context dummy;
        context_switch(&dummy, &tasks[0]);  
    } else {
        context_switch(&tasks[prev], &tasks[current_task]);
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