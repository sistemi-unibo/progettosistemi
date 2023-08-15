#include "include/pcb.h"
#include "include/ash.h"
#include "include/ns.h"
#include "include/pandos_const.h"
#include "include/pandos_types.h"
#include "include/types.h"
#include "include/exception.h"
#include <umps/libumps.h>

//da controllare
#define NUM_SEMAPHORES (DEVINTNUM+1) * DEVPERINT +1

extern void uTLB_RefillHandler();


int main(){
    
    //initialize phase 1 data structures
    initPcbs();
    initASH();
    initNamespaces();

    //initialize variables

    int processCount = 0; //number of alive processes
    int softBlockCount = 0; //number of processes in soft block

    static struct list_head readyQueue; 
    mkEmptyProcQ(&readyQueue); //ready queue processes

    pcb_t *currentProcess = NULL; //current active process

    // DA CONTROLLARE

    int subDeviceSemaphores[NUM_SEMAPHORES]; //array of semaphores for each subdevice
    
    //pass up vector initialization
    // gli indirizzi di memoria menzionati si trovano già in pandos_const.h
    // PASSUPVECTOR e KERNELSTACK

    passupvector_t *puv = (passupvector_t *) PASSUPVECTOR;
    puv->tlb_refill_handler = (memaddr) uTLB_RefillHandler;
    puv->tlb_refill_stackPtr = (memaddr) KERNELSTACK;
    puv->exception_handler = (memaddr) exceptionHandler;
    puv->exception_stackPtr = (memaddr) KERNELSTACK;

    LDIT(100000); //set timer to 100ms

    //alloc single process ...

    pcb_PTR proc = allocPcb();
    processCount++;
    //interrupts enabled, process local timer enabled, kernel mode on, 
    //SP set to RAMTOP, PC set to the address of test

    //state initialization
    state_t procState;
    STST(&procState);

    //set stack pointer to RAMTOP
    





    insertProcQ(&readyQueue, proc);
    return 0;
}
