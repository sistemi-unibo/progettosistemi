#include "include/exceptions.h"

extern pcb_t *currentProcess;
void exceptionHandler()
{

    // prendo la causa dell'eccezione e shifto i bit di 2
    int cause = getCAUSE() & GETEXECCODE;
    cause = cause >> CAUSESHIFT;

    switch (cause)
    {
    case 0:
        // interrupt handler
        interruptHandler();
        break;
    case 1 ... 3:
        // TLB exception handling
        passupordie(PGFAULTEXCEPT);
        break;

    case 8:
        // SYSCALL exception handler
        syscallHandler();
        break;

    // da 4 a 7 e da 9 a 12
    default:
        // program trap exception handler
        passupordie(GENERALEXCEPT);
        break;
    }
}

void passupordie(int index)
{
    //ricontrollare la &
    if (&(currentProcess->p_supportStruct) == NULL)
    {
        //probabilmente sbagliata
        terminate_process();

        //SYSCALL(2, &(currentProcess->p_pid), 0,0);
    }
    
}