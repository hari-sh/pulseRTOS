#include "tasks.h"
#include "uart.h"

void main(void) {
  uart_puts("Clean Scheduler Starting...\n");

  init_task(0, taskA);
  init_task(1, taskB);
  init_task(2, taskC);

  setTaskInit();

  while (1)
    ;
}
