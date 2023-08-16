#include "include/pcb.h"
#include "include/ash.h"
#include "include/ns.h"
#include "include/pandos_const.h"
#include "include/pandos_types.h"
#include "include/types.h"
#include "include/exceptions.h"
#include <umps/libumps.h>

//da controllare
#define NUM_SEMAPHORES (DEVINTNUM+1) * DEVPERINT +1

extern void uTLB_RefillHandler();
extern void test();


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
    //da implementare exceptionHandler
    puv->exception_handler = (memaddr) exceptionHandler;
    puv->exception_stackPtr = (memaddr) KERNELSTACK;

    LDIT(100000); //set timer to 100ms

    //alloc single process ...

    pcb_PTR proc = allocPcb();
    processCount++;
    /* Processor state 
    typedef struct state {
        unsigned int entry_hi;  no idea
        unsigned int cause;   cause register, contiene i bit di causa dell'eccezione
        unsigned int status;  status register, contiene i bit di stato del processore
        unsigned int pc_epc;   program counter register, contiene l'indirizzo della prossima istruzione da eseguire
        unsigned int gpr[STATE_GPR_LEN];  gpr -> general purpose registers, è l'array in cui si trovano i registri
        unsigned int hi;
        unsigned int lo;  two special registers used to hold the results of multiplication and division
        
    } state_t;
    */

    //state initialization
    state_t procState;
    STST(&procState);

    /*
    state_t.gpr[reg_sp] //stack pointer register
    controllare file types per verificare registro corretto
    per come sono dichiarati i registri, non bisogna scrivere gpr[reg_sp]
    ma direttamente il registro
    esempio:
    #define reg_sp  gpr[26]
    */
    //stack pointer set to RAMTOP
    RAMTOP(procState.reg_sp);
    
    /*
    un tipo assegna anche procState.reg_t9 = (memaddr) test;
    reg_t9 sarebbe gpr[24], non so che registro rappresenti
    program counter set to test
    anche nei file di test setta i registri t9 e pc_epc sempre assieme
    esempio:
    hp_p1state.pc_epc = hp_p1state.reg_t9 = (memaddr)hp_p1;
    */
    procState.pc_epc = (memaddr) test;

    /*
    queste cose si settano attraverso lo status register
    i vari valori sono definiti in pandos_const.h
    del tipo
    procState.status = ALLOFF | etc...
    sono ancora molto confusa da tutto ciò
    Status register constants
    #define ALLOFF      0x00000000     bit 0 (0000), IEc, global interrupt enable bit, when 0 all interrupts are disabled
    #define USERPON     0x00000008     bit 4 (1000)
    #define IEPON       0x00000004     bit 3 (0100)
    #define IECON       0x00000001     bit 0 (0001), IEc, when 1 interrupts on
    #define IMON        0x0000FF00     (1*8 0*8), IM, interrupt mask
    #define TEBITON     0x08000000     
    #define DISABLEINTS 0xFFFFFFFE
    */
    
    //interrupts enabled, process local timer enabled, kernel mode on
    

    //set stack pointer to RAMTOP
    

    insertProcQ(&readyQueue, proc);
    return 0;
}
