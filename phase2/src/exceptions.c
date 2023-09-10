#include "include/exceptions.h"

extern pcb_t *currentProcess;

void exceptionHandler()
{
    // taking exception cause and shifting bits of 2 to the right
    // because cause is the Cause.ExcCode, bits from 2 to 6 of the cause register
    int cause = getCAUSE() & GETEXECCODE;
    cause = cause >> CAUSESHIFT;

    // taking the state of the exception
    state_t *exceptionState = (state_t *)BIOSDATAPAGE;

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

    // from 4 to 7 and from 9 to 12
    default:
        // program trap exception handler
        passupordie(GENERALEXCEPT, exceptionState);
        break;
    }
}

void passupordie(int index, state_t *exceptionState)
{
    if (&(currentProcess->p_supportStruct) == NULL)
    {
        // if the process has no support struct it is terminated
        terminateProcess(currentProcess->p_pid);
    }
    else
    {
        // copying the state of the process that caused the exception in the support struct
        copyOfState(exceptionState, &(currentProcess->p_supportStruct->sup_exceptState[index]));

        // saving values of stack pointer, status and program counter in the support struct
        unsigned int stackPointer = currentProcess->p_supportStruct->sup_exceptContext[index].stackPtr;
        unsigned int status = currentProcess->p_supportStruct->sup_exceptContext[index].status;
        unsigned int pcounter = currentProcess->p_supportStruct->sup_exceptContext[index].pc;

        LDCXT(stackPointer, status, pcounter);
    }
}

void syscallHandler(state_t *exceptionState)
{
    // incrementing program counter by one word
    exceptionState->pc_epc += WORDLEN;

    exceptionState->reg_v0 = SYSCALL(exceptionState->reg_a0, exceptionState->reg_a1, exceptionState->reg_a2, exceptionState->reg_a3);

    // exception type is read from register a0
    int sysType = exceptionState->reg_a0;

    // and operation between status register and userpon should be 0
    // if the process is in kernel mode and different if it is in user mode
    if (exceptionState->status & USERPON != ALLOFF)
    {
        // user mode
        // sets the Cause.ExcCode a RI (reserved instruction) then calls exception handler

        exceptionState->cause |= (PRIVINSTR << CAUSESHIFT);
        passupordie(GENERALEXCEPT, exceptionState);
    }
    else
    {
        // kernel mode

        switch (sysType)
        {
        case CREATEPROCESS:
            // SYS1

            createProcess(exceptionState, currentProcess->p_supportStruct, currentProcess->namespaces);
            break;

        case TERMPROCESS:
            // SYS2
            terminateProcess(currentProcess->p_pid);
            break;

        case PASSEREN:
            // SYS3
            passeren(exceptionState, exceptionState->reg_a1);
            break;

        case VERHOGEN:
            // SYS4

            verhogen(exceptionState);
            break;

        case DOIO:
            // SYS5
            DoIo(exceptionState);
            break;

        case GETTIME:
            // SYS6
            get_CPU_Time(exceptionState);
            break;

        case CLOCKWAIT:
            // SYS7
            //   Wait_For_Clock();
            break;

        case GETSUPPORTPTR:
            // SYS8
            Get_Support_Data();
            break;

        case GETPROCESSID:
            // SYS9
            Get_Process_Id(exceptionState->reg_a1);
            break;

        case GETCHILDREN:
            // SYS10
            Get_Children(0, 0);
            break;

        default:
            // SYSCALLS 11 OR ABOVE
            passupordie(GENERALEXCEPT, exceptionState);
            break;
        }
    }
}

void copyOfState(state_t *source, state_t *dest)
{
    dest->entry_hi = source->entry_hi;
    dest->cause = source->cause;
    dest->status = source->status;
    dest->pc_epc = source->pc_epc;
    dest->hi = source->hi;
    dest->lo = source->lo;
    for (int i = 0; i < STATE_GPR_LEN; i++)
    {
        dest->gpr[i] = source->gpr[i];
    }
}
