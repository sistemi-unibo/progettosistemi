#include "include/exceptions.h"

extern pcb_t *currentProcess;

void exceptionHandler()
{
    // prendo la causa dell'eccezione e shifto i bit di 2
    //cause è la Cause.ExcCode, i bit da 2 a 6 del cause register
    int cause = getCAUSE() & GETEXECCODE;
    state_t* exceptionState = (state_t*)BIOSDATAPAGE;
    cause = cause >> CAUSESHIFT;

    switch (cause)
    {
    case 0:
        // interrupt handler
        interruptHandler();
        break;
    case 1 ... 3:
        // TLB exception handling
        passupordie(PGFAULTEXCEPT, exceptionState);
        break;

    case 8:
        // SYSCALL exception handler
        syscallHandler(exceptionState);
        break;

    // da 4 a 7 e da 9 a 12
    default:
        // program trap exception handler
        passupordie(GENERALEXCEPT, exceptionState);
        break;
    }
}

void passupordie(int index, state_t *exceptionState) //pag 22-23 phase2.book
{
    //ricontrollare la &
    if (&(currentProcess->p_supportStruct) == NULL)
    {
        //probabilmente sbagliata 
        terminate_process(); 
        //SYSCALL(2, &(currentProcess->p_pid), 0,0);
    }
    else{

    //da implementare copyState

        /* Copio lo stato del processo che ha causato l'eccezione nella support struct */
        copyOfState(exceptionState, &(currentProcess->p_supportStruct->sup_exceptState[index]));

        /* Salvo il valore dello stack pointer, dello status e del program counter nelle variabili*/
        unsigned int stackPointer =currentProcess->p_supportStruct->sup_exceptContext[index].stackPtr;
        unsigned int status = currentProcess->p_supportStruct->sup_exceptContext[index].status;
        unsigned int pcounter = currentProcess->p_supportStruct->sup_exceptContext[index].pc;

        /* Eseguo la macro che gestisce l'eccezione (penso passandola a un livello superiore) */
        LDCXT(stackPointer, status, pcounter);

    }
    
}

void syscallHandler(state_t *exceptionState) {
    //prendo l'exception state
    
    //leggo nel registro a0 il tipo di eccezione
    int sysType = exceptionState->reg_a0;

    
    //se è in user mode fai
    //la and tra lo status register dell'exceptionstate e userpon dee fare 0 se il processo è in kernel mode e diverso se è in user mode
    if(exceptionState->status & USERPON != ALLOFF){
        //sets the Cause.ExcCode a RI (reserved instruction) e poi chiama l'exception handler

        exceptionState->cause |= ( PRIVINSTR << CAUSESHIFT);
        passupordie(GENERALEXCEPT, exceptionState); 


    }

    //else se è in kernel mode:
    //switch in base al tipo
    //capire poi cosa passarci come parametri
    switch (sysType)
    {
    case CREATEPROCESS:
    //SYS1
        createProcess();
        break;
    
    case TERMPROCESS:
    //SYS2
        terminate_process();
        break;
    
    case PASSEREN:
    //SYS3
        passeren();
        break;

    case VERHOGEN:
    //SYS4
        veroghen();
        break;
    
    case DOIO:
    //SYS5
        Do_IO();
        break;
    
    case GETTIME:
    //SYS6
        get_cpu_time();
        break;
    
    case CLOCKWAIT:
    //SYS7
        wait_for_clock();
        break;

    case GETSUPPORTPTR:
    //SYS8
        get_support_data();
        break;

    case GETPROCESSID:
    //SYS9
        get_process_ID();
        break;

    case GETCHILDREN:
    //SYS10
        get_children();
        break;


    default:
    //SYSCALLS 11 OR ABOVE
        passupordie(GENERALEXCEPT, exceptionState);
        break;
    }
}

void copyOfState(state_t* source, state_t* dest){
   dest-> entry_hi = source ->entry_hi;
   dest-> cause = source ->cause;
   dest-> status = source ->status;
   dest-> pc_epc = source ->pc_epc;
   dest-> hi = source ->hi;
   dest-> lo = source ->lo;
   for (int i = 0; i < STATE_GPR_LEN; i++)
   {
        dest->gpr[i] = source->gpr[i];
   }

}


//implementare interruptHandler, tutte system call