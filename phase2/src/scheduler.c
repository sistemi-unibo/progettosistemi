#include "include/scheduler.h"
#define CONTINUECASE 1
#define HALTCASE 2
#define WAITCASE 3
#define PANICCASE 4
#define timeclock (TIMESLICE)*(*((cpu_t*)TIMESCALEADDR))

extern int processCount;
extern int softBlockCount;
extern struct list_head *readyQueue;
extern pcb_t* currentProcess;
extern int subDeviceSemaphores[NUM_SEMAPHORES];

int check_case(){
    if(!emptyProcQ(readyQueue)){
        return CONTINUECASE;
    }
    else if(processCount == 0){
        return HALTCASE;
    }
    else if(processCount > 0 && softBlockCount > 0){
        return WAITCASE;
    }
    else if(processCount > 0 && softBlockCount == 0)
    {
        return PANICCASE;
    }
}

void scheduler(){
    int cases = check_case();
    unsigned int status = getSTATUS();
    switch(cases){
        case CONTINUECASE:

        /*prendo in testa il primo proesso dalla readyQueue e lo uso come current
        la insert inserisce in coda e la remove toglie in testa, quindi è FIFO.
        setto il timer a 5 ms
        modifico lo stato del processore con quello del processo
        */
            currentProcess = removeProcQ(readyQueue);
            setTIMER(timeclock);
            LDST(&(currentProcess->p_s));
            break;
        case HALTCASE:
            HALT();
            break;
        case WAITCASE:

        //abilitiamo gli  interrupt
            status |= IEPON | IMON;

        //mettiamo il PLT  ad un numero molto alto
            setTIMER((__INT32_MAX__)*(*((cpu_t*)TIMESCALEADDR)));
        
        //settiamo lo status del processore
            setSTATUS(status);

            WAIT();
            break;
        case PANICCASE:
            PANIC();
            break;
    }
}