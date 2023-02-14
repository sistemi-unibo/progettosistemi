#ifndef _ASH_H
#define _ASH_H

#include "types.h"
#include "pandos_types.h"
#include "list.h"
#include "pcb.h"

semd_t semd_table[MAXPROC];
struct list_head* semdFree_h;
struct list_head* semd_h;


//gets specified semaphore according to key
semd_t* getsem(int *key);


/*Viene inserito il PCB puntato da p nella coda dei
processi bloccati associata al SEMD con chiave
semAdd. Se il semaforo corrispondente non è
presente nella ASH, alloca un nuovo SEMD dalla
lista di quelli liberi (semdFree) e lo inserisce nella
ASH, settando I campi in maniera opportuna (i.e.
key e s_procQ). Se non è possibile allocare un
nuovo SEMD perché la lista di quelli liberi è vuota,
restituisce TRUE. In tutti gli altri casi, restituisce
FALSE
*/

int insertBlocked(int * semAdd, pcb_t *p);

/*Viene inserito il PCB puntato da p nella coda dei
processi bloccati associata al SEMD con chiave
semAdd. Se il semaforo corrispondente non è
presente nella ASH, alloca un nuovo SEMD dalla
lista di quelli liberi (semdFree) e lo inserisce nella
ASH, settando I campi in maniera opportuna (i.e.
key e s_procQ). Se non è possibile allocare un
nuovo SEMD perché la lista di quelli liberi è vuota,
restituisce TRUE. In tutti gli altri casi, restituisce
FALSE
*/

pcb_t* removeBlocked(int *semAdd);

/*Rimuove il PCB puntato da p dalla coda del semaforo
su cui è bloccato (indicato da p->p_semAdd). Se il PCB
non compare in tale coda, allora restituisce NULL
(condizione di errore). Altrimenti, restituisce p. Se la
coda dei processi bloccati per il semaforo diventa
vuota, rimuove il descrittore corrispondente dalla ASH
e lo inserisce nella coda dei descrittori liberi
*/

pcb_t* outBlocked(pcb_t *p);

/*Restituisce (senza rimuovere) il puntatore al PCB che si
trova in testa alla coda dei processi associata al SEMD
con chiave semAdd. Ritorna NULL se il SEMD non
compare nella ASH oppure se compare ma la sua coda
dei processi è vuota.
*/
pcb_t* headBlocked(int *semAdd);

#endif