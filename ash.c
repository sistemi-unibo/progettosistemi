#include "ash.h"


//aux function used to get a semaphore from the sem_h list given the key
semd_t *getsem(int *key)
{
    struct hlist_node *x;
    hash_for_each(semd_h, key, x, s_link)
    {
        semd_t *sem = container_of(x, semd_t, s_link);
        if (sem->s_key == key)
        {
            return sem; // semaphore found
        }
    }
    return NULL; // no semaphore found
}

int insertBlocked(int *semAdd, pcb_t *p)
{
    semd_t *sem = getsem(semAdd);
    if (sem != NULL)   //semaphore found, inserts the pcb in semprocq
    {
        insertProcQ(&(sem->s_procq), p);
    }
    else
    {
        if (list_empty(semdFree_h))  //no more free semaphores to allocate
        {
            return TRUE;
        }
        else  //allocates a new semaphore
        {
            semd_t *new = semdFree_h->next;
            list_del(semdFree_h->next);  //deletes the first entry from the list of free semaphores
            semd_t *newsem = container_of(new, semd_t, s_link); //pics the free pcb and creates a new one for the active sem list
            hash_add();   //adds new sem to hashtable
            insertProcQ(&(newsem->s_procq), p);   //inserts pcb to tail of newsem procq list
            newsem->s_key = semAdd;
        }
    }
    p->p_semAdd = semAdd;
    return FALSE;
}

pcb_t *removeBlocked(int *semAdd)
{
}