#include "pandos_types.h"
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
