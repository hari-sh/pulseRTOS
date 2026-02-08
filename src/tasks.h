#ifndef TASKS_H
#define TASKS_H

#define MAX_TASKS 3
#define STACK_SIZE 1024

void init_task(int id, void (*fn)(void));
void schedule(void);
void setTaskInit(void);
int getTaskInit(void);

// Task entry wrapper
void task_entry(void (*fn)(void));

// Example tasks
void taskA(void);
void taskB(void);
void taskC(void);

#endif
