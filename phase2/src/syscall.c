#include "include/syscall.h"

extern int processCount;
extern pcb_t *currentProcess;
extern struct list_head *readyQueue;

//da controllare i vari casi in cui i parametri sono NULL e i casting
int createProcess (state_t *statep, support_t *supportp, nsd_t *ns){
    pcb_t *newProcess = allocPcb();
    if (newProcess == NULL) {
        &(statep->reg_v0) = NOPROC;
        LDST(statep);
    } else {
        //inserting new process in the process queue 
        insertProcQ(readyQueue, newProcess);

        processCount++;

        insertChild(currentProcess, newProcess);

        //da controllare, forse va fatto casting
        copyOfState(statep->reg_a1, &(newProcess->p_s));
        //da controllare, forse va fatto casting
        // da controllare come mai passiamo supportp come parametro
        //ma non viene utilizzato
        newProcess->p_supportStruct = statep->reg_a2;

        newProcess->p_time = 0;
        newProcess->p_semAdd = NULL;

        newProcess->namespaces = ns;

        LDST(statep);
    }

    
}