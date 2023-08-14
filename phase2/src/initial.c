#include "include/pcb.h"
#include "include/ash.h"
#include "include/ns.h"
#include "include/pandos_const.h"
#include "include/pandos_types.h"
#include "include/types.h"
#define NUM_SEMAPHORES (DEVINTNUM*2)
int main(){
    
    //initialize phase 1 data structures
    initPcbs();
    initASH();
    initNS();

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

    

    return 0;
}
