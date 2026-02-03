#include "uart.h"
#include "timer.h"
#include "tasks.h"

void main(void)
{
    uart_puts("Clean Scheduler Starting...\n");
    
    timer_init();
    
    init_task(0, taskA);
    init_task(1, taskB);
    init_task(2, taskC);
    
    start_scheduler();
    
    while(1);
}
