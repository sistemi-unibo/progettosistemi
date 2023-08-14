#include "include/pcb.h"
#include "include/ash.h"
#include "include/ns.h"
#include "include/pandos_const.h"
#include "include/pandos_types.h"
#include "include/types.h"

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

    // da inizializzare: Un semaforo (e.g. una variabile int) per ogni
    //(sub) dispositivo. Non necessariamente tutti
    //questi semafori sono sempre attivi.
    
    return 0;
}
