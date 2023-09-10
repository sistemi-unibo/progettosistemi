#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "pandos_const.h"
#include "pandos_types.h"
#include "types.h"
#include "pcb.h"
#include <umps/libumps.h>
#define NUM_SEMAPHORES (DEVINTNUM + 1) * DEVPERINT + 1

extern int processCount;
extern int softBlockCount;
extern struct list_head *readyQueue;
extern pcb_t *currentProcess;
extern int subDeviceSemaphores[NUM_SEMAPHORES];

// checks ready queue, process count and soft block count to determine the case
int check_case();

// scheduler function
void scheduler();

#endif
