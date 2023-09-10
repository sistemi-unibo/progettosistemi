#include "include/syscall.h"

int createProcess(state_t *statep, support_t *supportp, nsd_t *ns)
{
    pcb_t *newProcess = allocPcb();
    if (newProcess == NULL)
    {
        statep->reg_v0 = NOPROC;
        LDST(statep);
    }
    else
    {
        // inserting new process in the process queue
        insertProcQ(readyQueue, newProcess);

        processCount++;

        insertChild(currentProcess, newProcess);

        copyOfState(statep->reg_a1, &(newProcess->p_s));

        newProcess->p_supportStruct = statep->reg_a2;

        newProcess->p_time = 0;
        newProcess->p_semAdd = NULL;

        if (ns != NULL)
        {

            addNamespace(newProcess, ns);
        }
        else
        {
            // inherit from parent
            for (int i = 0; i < NS_TYPE_MAX; i++)
            {
                addNamespace(newProcess, currentProcess->namespaces[i]);
            }
        }
        LDST(statep);
    }

    return newProcess->p_pid;
}

void terminateProcess(int pid)
{
    pcb_t *terminateProc = NULL;

    if (pid == 0)
    {
        terminateProc = currentProcess;
    }
    else
    {

        terminateProc = searchProc(pid, readyQueue);
    }

    if (emptyChild(terminateProc))
    {
        outChild(terminateProc);
        freePcb(terminateProc);
        currentProcess--;
    }
    else
    {

        killChildren(terminateProc);
    }

    scheduler();
}

void killChildren(pcb_t *proc)
{

    if (emptyChild(proc))
    {
        outChild(proc);
        // ...
    }
    else
    {
        // recursive call on children
        while (!emptyChild(proc))
        {
            pcb_t *passChild = container_of((&proc->p_child)->next, pcb_t, p_sib);
            killChildren(passChild);
        }
    }
}

pcb_t *searchProc(int pid, struct list_head *procQueue)
{

    pcb_t *returnProc = NULL;
    struct list_head *x;

    list_for_each(x, procQueue)
    {
        if (container_of(x, pcb_t, p_list)->p_pid == pid)
        {
            returnProc = container_of(x, pcb_t, p_list);
        }
    }

    return returnProc;
}

void passeren(state_t *exceptionState, int *semAddr)
{

    if (*semAddr > 0)
    {
        *semAddr--;
        LDST(exceptionState);
    }
    else
    {
        insertBlocked(semAddr, currentProcess);
        scheduler();
    }
}

void verhogen(state_t *exceptionState)
{

    int *semAddr = exceptionState->reg_a1;
    pcb_t *removedproq = removeBlocked(semAddr);
    if (removedproq == NULL)
    {
        *semAddr++;
    }
    else
    {
        insertProcQ(readyQueue, removedproq);
    }
    LDST(exceptionState);
}

int DoIo(state_t *exceptionState)
{
    // command address
    int *cmdAddr = exceptionState->reg_a1;
    int *cmdValues = exceptionState->reg_a2;

    // cmdAddr - offset (start) / size of a single device register
    // to obtain device number
    int dev = (unsigned int)(cmdAddr - DEV_REG_START) / DEV_REG_SIZE;

    dtpreg_t *cmdAddrCasted;
    termreg_t *terminal;

    switch (dev)
    {
    case 0 ... 31:
        // non terminal, linee 3-6
        cmdAddrCasted = (dtpreg_t *)cmdAddr;
        cmdAddrCasted->status = cmdValues[0];
        cmdAddrCasted->command = cmdValues[1];
        cmdAddrCasted->data0 = cmdValues[2];
        cmdAddrCasted->data1 = cmdValues[3];
        break;
    case 32 ... 47:
        // terminal linea 7
        terminal = (termreg_t *)cmdAddr;

        if (dev % 2 == 0)
        {
            // receive
            terminal->recv_status = cmdValues[0];
            terminal->recv_command = cmdValues[1];
        }
        else
        {
            // transmit
            terminal->transm_status = cmdValues[0];
            terminal->transm_command = cmdValues[1];
        }

        break;
    default:
        return -1;
        break;
    }

    passeren(exceptionState, subDeviceSemaphores[dev]);

    return 0;
}

int get_CPU_Time(state_t *exceptionState)
{

    cpu_t actual_time;
    STCK(actual_time);

    // ...
}

support_t *Get_Support_Data()
{
    support_t *supportDataPtr = NULL;

    if (currentProcess->p_supportStruct != NULL)
    {
        supportDataPtr = currentProcess->p_supportStruct;
    }

    return (supportDataPtr);
}

int Get_Process_Id(int parent)
{

    if (parent == 0)
    {
        return (currentProcess->p_pid);
    }
    else
    {
        if (currentProcess->namespaces == currentProcess->p_parent->namespaces)
        {
            return (currentProcess->p_parent->p_pid);
        }
        else
        {
            return (0);
        }
    }
}

int Get_Children(int *children, int size)
{
    // ...
}
