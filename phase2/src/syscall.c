#include "include/syscall.h"

extern int processCount;
extern pcb_t *currentProcess;
extern struct list_head *readyQueue;

//da controllare i vari casi in cui i parametri sono NULL e i casting
int createProcess (state_t *statep, support_t *supportp, nsd_t *ns){
    pcb_t *newProcess = allocPcb();
    if (newProcess == NULL) {
        statep->reg_v0 = NOPROC;
        LDST(statep);
    } else {
        //inserting new process in the process queue 
        insertProcQ(readyQueue, newProcess);

        processCount++;

        insertChild(currentProcess, newProcess);

        //da controllare, forse va fatto casting
        copyOfState(statep->reg_a1, &(newProcess->p_s));
        //da controllare, forse va fatto casting
        // da controllare come mai passiamo supportp come parametro
        //ma non viene utilizzato
        newProcess->p_supportStruct = statep->reg_a2;

        newProcess->p_time = 0;
        newProcess->p_semAdd = NULL;
    
        if( ns != NULL){
            //addNs ritorna TRUE o FALSE, da controllare
            addNamespace(newProcess, ns);
        } else{
            //eredita i namespace del padre
            for(int i=0 ; i < NS_TYPE_MAX; i++){
                addNamespace(newProcess, currentProcess->namespaces[i]);
            }
        }
        LDST(statep);
    }

    return newProcess->p_pid;
}


//controllare caso in cui searchProc restituisce NULL 
//(nessun proc con id = pid trovato)
void terminateProcess(int pid){
    pcb_t *terminateProc = NULL;

    if ( pid == 0){
        terminateProc = currentProcess;
    } else {

        terminateProc = searchProc(pid, readyQueue);
    }

    if(emptyChild(terminateProc)){
        outChild(terminateProc);
        freePcb(terminateProc);
        currentProcess--;
    } else {

        killChildren(terminateProc);
    }

    scheduler();
}

void killChildren(pcb_t *proc){

    
    if (emptyChild(proc))
    {
        //non ha figli, uccido processo
        //primo punto, rimuovo proc dalla lista dei figli del padre
        outChild(proc);
        //svariati altri punti...
        
    } else {
    // ha figli, chiamata ricorsiva su di essi
        while(!emptyChild(proc)){
            /*
            DA CONTROLLARE
            bisogna passare il figlio di proc alla funzione killChildren
            ma proc->p_child è di tipo list_head, la funzione vuole pcb_t
            sono estremamente dubbiosa del funzionamento di questo container of
            */
            pcb_t *passChild = container_of(proc, pcb_t, p_child);

            killChildren(passChild);
        }
    }
    

}

pcb_t *searchProc(int pid, struct list_head *procQueue){

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