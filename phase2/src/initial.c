#include <stdio.h>
#include "include/pcb.h"
#include "include/ash.h"
#include "include/ns.h"
#include "include/pandos_const.h"
#include "include/pandos_types.h"
#include "include/types.h"
#include "include/scheduler.h"
#include "include/exceptions.h"

extern void uTLB_RefillHandler();
extern void test();

int processCount = 0;   // number of alive processes
int softBlockCount = 0; // number of processes in soft block
struct list_head *readyQueue;
pcb_t *currentProcess = NULL;            // current active process
int subDeviceSemaphores[NUM_SEMAPHORES]; // array of semaphores for each subdevice

// empties support struct
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

void *memcpy(void *dest, const void *src, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        ((char *)dest)[i] = ((char *)src)[i];
    }
}

int main()
{

    // initialize phase 1 data structures
    initPcbs();
    initASH();
    initNamespaces();

    // initialize ready queue processes
    mkEmptyProcQ(readyQueue);

    // semaphores initialization
    for (int i = 0; i < NUM_SEMAPHORES; i++)
    {
        subDeviceSemaphores[i] = 0;
    }

    // pass up vector initialization
    passupvector_t *puv = (passupvector_t *)PASSUPVECTOR;
    puv->tlb_refill_handler = (memaddr)uTLB_RefillHandler;
    puv->tlb_refill_stackPtr = (memaddr)KERNELSTACK;
    puv->exception_handler = (memaddr)exceptionHandler;
    puv->exception_stackPtr = (memaddr)KERNELSTACK;

    // set timer to 100ms
    LDIT(100000);

    // alloc single process
    pcb_PTR proc = allocPcb();
    processCount++;

    // state initialization
    state_t procState;
    STST(&procState);

    // set stack pointer to the top of the kernel stack
    RAMTOP(procState.reg_sp);

    // program counter set to test
    procState.pc_epc = procState.reg_t9 = (memaddr)test;

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

    insertProcQ(readyQueue, proc);

    scheduler();
    return 0;
}
