#ifndef PCB_H
#define PCB_H

#include "types.h"
#include "pandos_types.h"
#include "list.h"



// pcb double linked list functions

/*
To initialize the pcbFree List:
Function 1
Inizializza la lista pcbFree in modo da contenere tutti gli elementi della
pcbFree_table. Questo metodo deve essere chiamato una volta sola in fase di
inizializzazione della struttura dati.
*/
void initPcbs();

/*
pcbs which are no longer in use can be returned to the pcbFree list by
using the method:
Function 2
Inserisce il PCB puntato da p nella lista
dei PCB liberi (pcbFree_h)
*/
void freePcb(pcb_t *p);

/*
pcbs should be allocated by using:
Function 3
Restituisce NULL se la pcbFree_h è vuota. Altrimenti rimuove un elemento dalla
pcbFree, inizializza tutti i campi (NULL/0) e restituisce l’elemento rimosso.
*/
pcb_t *allocPcb();
/*
This method is used to initialize a variable to be tail pointer
to a process queue.
Return a pointer to the tail of an empty process queue; i.e. NULL
Function 4
Crea una lista di PCB, inizializzandola come lista vuota
*/
void mkEmptyProcQ(struct list_head *head);

/*
Function 5
Restituisce TRUE se la lista puntata da head è vuota, FALSE altrimenti.
*/
int emptyProcQ(struct list_head *head);

/*
Function 6
Inserisce l’elemento puntato da p nella coda dei processi puntata da head.
*/
void insertProcQ(struct list_head *head, pcb_t *p);

/*
Function 7
Restituisce l’elemento di testa della coda dei processi da head,
SENZA RIMUOVERLO. Ritorna NULL se la coda non ha elementi.
*/
pcb_t *headProcQ(struct list_head *head);

/*
Function 8
Rimuove il primo elemento dalla coda dei processi puntata da head. Ritorna NULL se la
coda è vuota. Altrimenti ritorna il puntatore all’elemento rimosso dalla lista.
*/
pcb_t *removeProcQ(struct list_head *head);

/*
Function 9
Rimuove il PCB puntato da p dalla coda dei processi puntata da head. Se p non è presente
nella coda, restituisce NULL. (NOTA: p può trovarsi in una posizione arbitraria della coda).
*/
pcb_t *outProcQ(struct list_head *head, pcb_t *p);



// pcb double linked tree functions

/*
Function 10
Restituisce TRUE se il PCB puntato da p non ha figli, FALSE altrimenti.
*/
int emptyChild(pcb_t *p);

/*
Function 11
Inserisce il PCB puntato da p come figlio del PCB puntato da prnt
*/
void insertChild(pcb_t *prnt, pcb_t *p);

/*
Function 12
Rimuove il primo figlio del PCB puntato da p. Se p non ha figli, restituisce NULL
*/
pcb_t *removeChild(pcb_t *p);

/*
Function 13
Rimuove il PCB puntato da p dalla lista dei figli del padre. Se il PCB puntato da
p non ha un padre, restituisce NULL, altrimenti restituisce l’elemento
rimosso (cioè p). A differenza della removeChild, p può trovarsi in una
posizione arbitraria (ossia non è necessariamente il primo figlio del padre)
*/
pcb_t *outChild(pcb_t *p);


#endif