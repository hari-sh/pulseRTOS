#include "timer.h"
#include "uart.h"
#include "tasks.h"

void timer_init(void)
{
    // 0x100000 is approx 1,000,000. 
    // At 10MHz, this is 1.0 seconds (10Hz).
    unsigned long long next = *MTIME + 10000000;
    
    *MTIMECMP = next;
}

// Called from trap.s
void trap_handler(void)
{
    unsigned int mcause;
    asm volatile("csrr %0, mcause" : "=r"(mcause));
    if ((mcause & 0x80000007) == 0x80000007) {
        timer_init(); // Ack/Rearm
        if (getTaskInit()) {
            schedule();
        }
    }
}
