#pragma once
#include "exceptions.h"
#include "ns.h"

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
