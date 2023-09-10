#include "include/scheduler.h"
#define CONTINUECASE 1
#define HALTCASE 2
#define WAITCASE 3
#define PANICCASE 4
#define timeclock (TIMESLICE) * (*((cpu_t *)TIMESCALEADDR))

int check_case()
{
    if (!emptyProcQ(readyQueue))
    {
        return CONTINUECASE;
    }
    else if (processCount == 0)
    {
        return HALTCASE;
    }
    else if (processCount > 0 && softBlockCount > 0)
    {
        return WAITCASE;
    }
    else if (processCount > 0 && softBlockCount == 0)
    {
        return PANICCASE;
    }
}

void scheduler()
{
    int cases = check_case();
    unsigned int status = getSTATUS();

    switch (cases)
    {
    case CONTINUECASE:

        // taking the first process from the readyQueue and using it as current
        currentProcess = removeProcQ(readyQueue);
        // setting the timer to 5 ms
        setTIMER(timeclock);
        // modifying the processor status with that of the process
        LDST(&(currentProcess->p_s));
        break;
    case HALTCASE:
        HALT();
        break;
    case WAITCASE:

        // abiliting interrupts
        status |= IEPON | IMON;

        // setting PLT very high
        setTIMER((__INT32_MAX__) * (*((cpu_t *)TIMESCALEADDR)));

        // setting processor status
        setSTATUS(status);

        WAIT();
        break;
    case PANICCASE:
        PANIC();
        break;
    }
}