#ifndef STACK_H
#define STACK_H

#include "tcb.h"

#define STACK_PATTERN   0xA5
#define STACK_GUARD     0xDEADBEEF
#define STACK_ALIGN     16


void check_stack_overflow(tcb_t *t);
void update_stack_high_water(tcb_t *t);
void dump_stack_usage(void);

#endif
