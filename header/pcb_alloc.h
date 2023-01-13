#include "pandos_types.h"
/*
Function 1
Inizializza la lista pcbFree in modo da contenere tutti gli elementi della
pcbFree_table. Questo metodo deve essere chiamato una volta sola in fase di
inizializzazione della struttura dati.
*/
void initPcbs();

/*
Function 2
Inserisce il PCB puntato da p nella lista
dei PCB liberi (pcbFree_h)
*/
void freePcb(pcb_t *p);

/*
Function 3
Restituisce NULL se la pcbFree_h è vuota. Altrimenti rimuove un elemento dalla
pcbFree, inizializza tutti i campi (NULL/0) e restituisce l’elemento rimosso.
*/

pcb_t *allocPcb();
