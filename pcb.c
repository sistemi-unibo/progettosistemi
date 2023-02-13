#include "pcb.h"

void initPcbs()
{
    INIT_LIST_HEAD(&pcbFree);
    pcbFree_h = &pcbFree;
    for (int i = 1; i < MAXPROC; i++)
    {
        list_add(&pcbFree_table[i].p_list, pcbFree_h);
    }
}

void freePcb(pcb_t *p)
{
    list_add_tail(&(p->p_list), pcbFree_h);
}

pcb_t *allocPcb()
{
    if (list_empty(pcbFree_h))
    {
        return NULL;
    }
    else
    {
        struct list_head *new = pcbFree_h->next;
        list_del(pcbFree_h->next);
        pcb_t *pcb = container_of(new, pcb_t, p_list);
        pcb->p_parent = NULL;
        INIT_LIST_HEAD(&pcb->p_child);
        INIT_LIST_HEAD(&pcb->p_sib);
        pcb->p_semAdd = NULL;
        pcb->p_time = 0;
        pcb->namespaces[NS_TYPE_MAX] = NULL;
        pcb->p_s.cause = 0;
        pcb->p_s.entry_hi = 0;
        pcb->p_s.hi = 0;
        pcb->p_s.lo = 0;
        pcb->p_s.pc_epc = 0;
        pcb->p_s.status = 0;
        for (int i = 0; i < STATE_GPR_LEN; i++)
        {
            pcb->p_s.gpr[i] = 0;
        }

        return pcb;
    }
}

void mkEmptyProcQ(struct list_head *head)
{
    if (head != NULL)
    {
        INIT_LIST_HEAD(head); // se head==NULL, non fa nulla; se head è diverso da NULL, usa la funzione INIT_LIST_HEAD (definita dal prof in list.h), che prendendo in input "head" (l'elemento sentinella), la inizializza impostando i puntatori avanti e indietro in modo che puntino all'elemento sentinella stesso (perché per ora la lista è vuota, c'è solo l'elemento sentinella head)
    }
}

int emptyProcQ(struct list_head *head)
{
    if (head == NULL)
    {
        return true; // se non esiste head, l'elemento sentinella, la lista è vuota.
    }

    return (list_empty(head)); // "list_empty" è la funzione data dal prof in "list.h" che abbiamo usato anche nella funzione "allocPcb" che controlla se head->next è uguale a head (in tal caso nella lista è presente solo la sentinella head, quindi la lista è di fatto vuota)
}

void insertProcQ(struct list_head *head, pcb_t *p)
{
    if (head != NULL && p != NULL)
    {

        list_add_tail(&p->p_list, head); // "list_add_tail" prende come parametro due tipi "list_head*": "head" è di tipo "list_head*", quindi posso passarglielo come parametro, mentre "p" è di tipo "pcb_t*", che è il puntatore a una struct che contiene altre struct, tra cui "list_head". L'elemento di tipo "list_head", all'interno di "p", si chiama "p_list", quindi come parametro a "list_add_tail" passo "p->p_list"
    }                                    // la soluzione di foxy mette &p->p_list a riga 66, ma non ho capito la motivazione ("list_add_tail" prende come parametri due puntatori a "list_head")
} // inoltre Foxy mette prima di "list_add_tail" una funzione creata da lui chiamata "list_sdel", definita nel suo file "util.h", che a quanto ho capito elimina i rapporti (prev e next) che p aveva nella pcbfreetable (DI QUESTA COSA NON SONO SICURO)

pcb_t *headProcQ(struct list_head *head)
{
    if (head == NULL || list_empty(head))
    {
        return NULL;
    }
    else
    {
        return container_of(head->next, pcb_t, p_list); // container_of: estrae un membro di una struct da quest'ultima. Prende come paraemtro: un puntatore al membro della struct da estrarre, il tipo della struttura che contiene il membro da estrarre; il nome del membro della struct che devo estrarre
                                                        //  Foxy usa "list_next(head)", definita in list.h. (A quanto ho capito "list_next" dato un puntatore a un elemento della lista, restituisce il next)
    }
}

pcb_t *removeProcQ(struct list_head *head)
{
    if (head == NULL || list_empty(head))
    {
        return NULL;
    }
    else
    {
        struct list_head *p = head->next;
        list_del(head->next);
        return container_of(p, pcb_t, p_list);
    }
}

pcb_t *outProcQ(struct list_head *head, pcb_t *p)
{
    if (head == NULL || list_empty(head))
    {
        return NULL;
    }
    else
    {
        struct list_head *x;
        list_for_each(x, head)
        {
            if (container_of(x, pcb_t, p_list) == p)
            {
                list_del(x);
                return container_of(x, pcb_t, p_list);
            }
        }
    }
    return NULL; // non ha trovato il pcb
}

int emptyChild(pcb_t *p)
{
    if (p == NULL)
    {
        return true;
    }

    return list_empty(&p->p_child); // list_empty controlla se "p->p_child" punta a "p": se sì vuol dire che p non ha figli e il puntatore che da "p" va ai figli, torna indietro; se "p->child" punta a qualcosa di diverso da p, vuol dire che p ha dei figli e quindi "list_empty" ritorna "false"
}

void insertChild(pcb_t *prnt, pcb_t *p)
{
    if (prnt != NULL && p != NULL)
    {
        p->p_parent = prnt;
        list_add_tail(&p->p_sib, &prnt->p_child);
    }
}

pcb_t *removeChild(pcb_t *p)
{
    if (list_empty(&p->p_child) || p == NULL)
    {
        return NULL;
    }
    else
    {
        struct list_head *tmp = (&p->p_child)->next;
        list_del((&p->p_child)->next);
        return container_of(tmp, pcb_t, p_sib);
    }
}

pcb_t *outChild(pcb_t *p)
{
    if (p == NULL || &p->p_parent == NULL)
    {
        return NULL;
    }
    else
    {
        p->p_parent = NULL;
        list_del(&p->p_sib);
        return p;
    }
}