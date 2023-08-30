#include <stdio.h>
#include "include/pcb.h"
#include "include/ash.h"
#include "include/ns.h"
#include "include/pandos_const.h"
#include "include/pandos_types.h"
#include "include/types.h"
#include "scheduler.h"
#include "include/exceptions.h"

void *memcpy(void *dest, const void *src, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        ((char *)dest)[i] = ((char *)src)[i];
    }
}
// da controllare

extern void uTLB_RefillHandler();
extern void test();

void nullifySupport_t(support_t *p_supportStruct)
{
    p_supportStruct->sup_asid = 0;
    for (int i = 0; i < 2; i++)
    {
        p_supportStruct->sup_exceptState[i].cause = 0;
        p_supportStruct->sup_exceptState[i].entry_hi = 0;
        for (int j = 0; j < 30; j++)
        {
            p_supportStruct->sup_exceptState[i].gpr[j] = 0;
        }
        p_supportStruct->sup_exceptState[i].hi = 0;
        p_supportStruct->sup_exceptState[i].lo = 0;
        p_supportStruct->sup_exceptState[i].pc_epc = 0;
        p_supportStruct->sup_exceptState[i].status = 0;

        p_supportStruct->sup_exceptContext[i].pc = 0;
        p_supportStruct->sup_exceptContext[i].status = 0;
        p_supportStruct->sup_exceptContext[i].stackPtr = 0;
    }
    for (int k = 0; k < MAXPAGES; k++)
    {
        p_supportStruct->sup_privatePgTbl[k].pte_entryHI = 0;
        p_supportStruct->sup_privatePgTbl[k].pte_entryLO = 0;
    }
}

int processCount = 0;   // number of alive processes
int softBlockCount = 0; // number of processes in soft block
struct list_head *readyQueue;
pcb_t *currentProcess = NULL;            // current active process
int subDeviceSemaphores[NUM_SEMAPHORES]; // array of semaphores for each subdevice

int main()
{

    // initialize phase 1 data structures
    initPcbs();
    initASH();
    initNamespaces();

    // initialize variables

    // controllare se bisogna metterla fuori dal main
    mkEmptyProcQ(readyQueue); // ready queue processes

    // DA CONTROLLARE

    // semaphores initialization
    // controllare se bisogna metterla fuori dal main
    for (int i = 0; i < NUM_SEMAPHORES; i++)
    {
        subDeviceSemaphores[i] = 0;
    }

    // pass up vector initialization
    //  gli indirizzi di memoria menzionati si trovano già in pandos_const.h
    //  PASSUPVECTOR e KERNELSTACK

    passupvector_t *puv = (passupvector_t *)PASSUPVECTOR;
    puv->tlb_refill_handler = (memaddr)uTLB_RefillHandler;
    puv->tlb_refill_stackPtr = (memaddr)KERNELSTACK;
    puv->exception_handler = (memaddr)exceptionHandler;
    puv->exception_stackPtr = (memaddr)KERNELSTACK;

    LDIT(100000); // set timer to 100ms

    // alloc single process ...

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

    // state initialization
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
    // stack pointer set to RAMTOP
    RAMTOP(procState.reg_sp);

    /*
    da controllare anche procState.reg_t9 = (memaddr) test;
    reg_t9 sarebbe gpr[24], non so che registro rappresenti
    program counter set to test
    anche nei file di test setta i registri t9 e pc_epc sempre assieme
    esempio:
    hp_p1state.pc_epc = hp_p1state.reg_t9 = (memaddr)hp_p1;
    */
    // program counter settato a test
    procState.pc_epc = procState.reg_t9 = (memaddr)test;

    /*
    queste cose si settano attraverso lo status register
    i vari valori sono definiti in pandos_const.h
    del tipo
    procState.status = ALLOFF | etc...
    sono ancora molto confusa da tutto ciò
    Status register constants
    #define ALLOFF      0x00000000
    #define USERPON     0x00000008     bit 4 (1000)
    #define IEPON       0x00000004     bit 3 (0100) global interrupt enable bit
    #define IECON       0x00000001
    #define IMON        0x0000FF00     (1*8 0*8), IM, interrupt mask
    #define TEBITON     0x08000000
    #define DISABLEINTS 0xFFFFFFFE
    */

    // interrupts enabled, process local timer enabled, kernel mode on
    procState.status = ALLOFF | IEPON | IMON | TEBITON;

    // the processor state structure is set to our created state
    proc->p_s = procState;

    // set all tree fields to  NULL
    proc->p_parent = NULL;
    INIT_LIST_HEAD(&proc->p_child);
    INIT_LIST_HEAD(&proc->p_sib);

    // p_time to 0
    proc->p_time = 0;

    // p_semadd to NULL
    proc->p_semAdd = NULL;

    // support_t initialized
    nullifySupport_t(proc->p_supportStruct);

    // nel caso non funzioni fare proc->p_supportStruct = NULL

    insertProcQ(readyQueue, proc);

    scheduler();
    return 0;
}
