#ifndef SYSCALL_H
#define SYSCALL_H

#include "exceptions.h"
#include "ns.h"
#include "ash.h"

extern int processCount;
extern pcb_t *currentProcess;
extern struct list_head *readyQueue;

/*
First system call:
creates a process and inserts it in the ready queue
this process is going to be the child of the current process
*/
int createProcess (state_t *statep, support_t *supportp, nsd_t *ns);
/*
Second system call:
terminates a process and all its children
if pid == 0, terminates the current process otherwise terminates the process 
corresponding to the pid
*/
void terminateProcess(int pid);
/*
Recursive function that terminates all the children of a process
*/
void killChildren(pcb_t *proc);
/*
Searches for a process with a given pid in a given queue
*/
pcb_t *searchProc(int pid, struct list_head *procQueue);


void passeren(state_t *exceptionState);

void verhogen(state_t *exceptionState);

int DoIo(state_t *exceptionState);

int get_CPU_Time(state_t *exceptionState);

support_t* Get_Support_Data();

int Get_Process_Id(int parent);

int Get_Children(int *children, int size);

#endif