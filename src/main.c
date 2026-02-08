#include "tasks.h"
#include "uart.h"
#include "timer.h"

void kernel_mem_init(void);

void main(void) {
    uart_puts("Clean Scheduler Starting...\n");

    kernel_mem_init();     // <<< THIS WAS MISSING
    timer_init();          // start timer

    init_task(0, taskA);
    init_task(1, taskB);
    init_task(2, taskC);

    setTaskInit();

    schedule();            // kick first task

    while (1);
}


