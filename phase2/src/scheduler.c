#include "include/scheduler.h"

extern int processCount;
extern int softBlockCount;
extern struct list_head readyQueue;
extern pcb_t* currentProcess;
extern int subDeviceSemaphores[NUM_SEMAPHORES];

