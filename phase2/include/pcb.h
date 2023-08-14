#ifndef PCB_H
#define PCB_H

#include "types.h"
#include "pandos_types.h"
#include "list.h"



// pcb double linked list functions
/*
Initialize the pcbFree list to contain all the elements of
the static array of MAXPROC pcbs. This method will be
called only once during data structure initialization
*/
void initPcbs();

/*
Pcbs which are no longer in use can be returned to the pcbFree list by
using this method.
Inserts the element pointed to by p onto the pcbFree list.
*/
void freePcb(pcb_t *p);

/*
Pcbs should be allocated by using this function.
Returns NULL if the pcbFree list is empty. Otherwise, re-
moves an element from the pcbFree list, provides initial values
for ALL of the pcbs fields (i.e. NULL and/or 0) and then
returns a pointer to the removed element. pcbs get reused, so
it is important that no previous value persist in a pcb when
it gets reallocated.
*/
pcb_t *allocPcb();

/*
This method is used to initialize a variable to be tail pointer
to a process queue.
Creates a pcb list, initializing it as empty.
*/
void mkEmptyProcQ(struct list_head *head);

/*
Returns TRUE if the queue whose tail is pointed to by head is
empty. Returns FALSE otherwise.
*/
int emptyProcQ(struct list_head *head);

/*
Inserts the pcb pointed by p into the process queue pointed by head.
*/
void insertProcQ(struct list_head *head, pcb_t *p);

/*
Returns a pointer to the first pcb from the process queue pointed by head.
Does not remove this pcb from the process queue. 
Returns NULL if the process queue is empty.
*/
pcb_t *headProcQ(struct list_head *head);

/*
Removes the first pcb from the process queue pointed by head.
Return NULL if the queue is empty.
Otherwise returns the pointer of the removed pcb.
*/
pcb_t *removeProcQ(struct list_head *head);

/*
Removes the pcb pointed by p from the process queue pointed by head.
Return NULL if the process is not in the queue.
*/
pcb_t *outProcQ(struct list_head *head, pcb_t *p);

// pcb double linked tree functions

/*
Returns TRUE if the pcb pointed by p doesn't have children, FALSE otherwise.
*/
int emptyChild(pcb_t *p);

/*
Inserts the pcb pointed by p as son of the pcb pointed by prnt
*/
void insertChild(pcb_t *prnt, pcb_t *p);

/*
Removes the first child of the pcb pointed by p.
Returns NULL if p doen't have children.
*/
pcb_t *removeChild(pcb_t *p);

/*
Removes the pcb pointed by p from the list of the father's children.
Returns NULL if the pcb pointed by p doesn't have a father, 
otherwise returns p.
*/
pcb_t *outChild(pcb_t *p);


#endif