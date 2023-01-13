#include "types.h"
#include "pandos_types.h"
/*
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
pcb_t headProcQ(struct list_head *head);

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