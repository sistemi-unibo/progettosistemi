#include "include/syscall.h"

// da controllare i vari casi in cui i parametri sono NULL e i casting
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

        // da controllare, forse va fatto casting
        copyOfState(statep->reg_a1, &(newProcess->p_s));
        // da controllare, forse va fatto casting
        //  da controllare come mai passiamo supportp come parametro
        // ma non viene utilizzato
        newProcess->p_supportStruct = statep->reg_a2;

        newProcess->p_time = 0;
        newProcess->p_semAdd = NULL;

        if (ns != NULL)
        {
            // addNs ritorna TRUE o FALSE, da controllare
            addNamespace(newProcess, ns);
        }
        else
        {
            // eredita i namespace del padre
            for (int i = 0; i < NS_TYPE_MAX; i++)
            {
                addNamespace(newProcess, currentProcess->namespaces[i]);
            }
        }
        LDST(statep);
    }

    return newProcess->p_pid;
}

// controllare caso in cui searchProc restituisce NULL
//(nessun proc con id = pid trovato)
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
        // non ha figli, uccido processo
        // primo punto, rimuovo proc dalla lista dei figli del padre
        outChild(proc);
        // svariati altri punti...
    }
    else
    {
        // ha figli, chiamata ricorsiva su di essi
        while (!emptyChild(proc))
        {
            /*
            DA CONTROLLARE
            bisogna passare il figlio di proc alla funzione killChildren
            ma proc->p_child è di tipo list_head, la funzione vuole pcb_t
            sono estremamente dubbiosa del funzionamento di questo container of
            */
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

// da ricontrollare
// controllare se ci va il copystate
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

// da ricontrollare
void verhogen(state_t *exceptionState)
{
    // prendo l'indirizzo del semaforo
    int *semAddr = exceptionState->reg_a1;
    pcb_t *removedproq = removeBlocked(semAddr);
    if (removedproq == NULL)
    {
        *semAddr++;
    }
    else
    {
        // controllare se serve la &
        insertProcQ(readyQueue, removedproq);
    }
    LDST(exceptionState);
}

int DoIo(state_t *exceptionState)
{ // non finita

    // command address
    int *cmdAddr = exceptionState->reg_a1;
    int *cmdValues = exceptionState->reg_a2;
    int size = sizeof(cmdValues) / sizeof(cmdValues[0]);
    int dev = (unsigned int)(cmdAddr - DEV_REG_START) / DEV_REG_SIZE;

    dtpreg_t *cmdAddrCasted;
    termreg_t *terminal;
    if (size == 2)
    {
        // terminal
    }
    else
    {
        // non terminal
    }

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
{ // non finita

    cpu_t actual_time;
    STCK(actual_time);

    // currentProcess->p_time += (actual_time - );

    // altrimenti p_time = 5000 - getTIMER ??
}

support_t *Get_Support_Data()
{ // da capire se prima del return devo chiamare LSDT(exceptionState). In questo caso passo "state_t* exceptionState" alla funzione come parametro
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
        { // non sono sicuro che si faccia così a controllare che il namespace del parent è uguale al namespace del figlio (perché "namespaces" è una lista e non so se devo controllare che tutta la lista del parent sia uguale a tutta la lista del figlio, oppure devo confrontare solo un elemento e nel caso quale?)
            return (currentProcess->p_parent->p_pid);
        }
        else
        {
            return (0);
        }
    }
}

int Get_Children(int *children, int size)
{ // siccome la funzione dovrebbe ritornare un array, ma questo in c non è possibile devo usare array dinamici con puntatori e devo rivedere come si usano perché non mi ricordo
}
