#include "include/scheduler.h"
#define CONTINUECASE 1
#define HALTCASE 2
#define WAITCASE 3
#define PANICCASE 4

extern int processCount;
extern int softBlockCount;
extern struct list_head readyQueue;
extern pcb_t* currentProcess;
extern int subDeviceSemaphores[NUM_SEMAPHORES];

int check_case(){
    if(!emptyProcQ(&readyQueue)){
        return CONTINUECASE;
    }
    else if(processCount == 0){
        return HALTCASE;
    }
    else if(processCount > 0 && softBlockCount > 0){
        return WAITCASE;
    }
    else{
        return PANICCASE;
    }
}

void scheduler(){
    int cases = check_case();
    switch(cases){
        case CONTINUECASE:
            
            break;
        case HALTCASE:
            HALT();
            break;
        case WAITCASE:
            WAIT();
            break;
        case PANICCASE:
            PANIC();
            break;
    }
}