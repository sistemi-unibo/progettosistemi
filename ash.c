#include "ash.h"

static semd_t semd_table[MAXPROC];
static struct list_head *semdFree_h;
DEFINE_HASHTABLE(semd_h, 10);
// aux function used to get a semaphore from the sem_h list given the key
semd_t *getsem(int *key)
{
    struct semd_t *x;
    int j = 0;
    hash_for_each(semd_h, j, x, s_link)
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
    if (sem != NULL) // semaphore found, inserts the pcb in semprocq
    {
        insertProcQ(&(sem->s_procq), p);
    }
    else
    {
        if (list_empty(semdFree_h)) // no more free semaphores to allocate
        {
            return TRUE;
        }
        else // allocates a new semaphore
        {
            semd_t *new = semdFree_h->next;
            list_del(semdFree_h->next);             // deletes the first entry from the list of free semaphores
            hash_add(semd_h, &new->s_link, semAdd); // adds new sem to hashtable
            insertProcQ(&(new->s_procq), p);        // inserts pcb to tail of newsem procq list
            new->s_key = semAdd;
        }
    }
    p->p_semAdd = semAdd;
    return FALSE;
}

pcb_t *removeBlocked(int *semAdd)
{
    semd_t *sem = getsem(semAdd);
    if (sem == NULL)
    {
        return NULL;
    }
    else
    {
        struct pcb_t *pcbdel = removeProcQ(&(sem->s_procq));
        if (list_empty(&(sem->s_procq)))
        {
            hash_del(&(sem->s_link));
            list_add(&(sem->s_freelink), semdFree_h);
        }
        return pcbdel;
    }
}

pcb_t *outBlocked(pcb_t *p)
{
    semd_t *sem = p->p_semAdd;

    if (sem == NULL)
    {
        return NULL;
    }
    else
    {
        pcb_t *p_out = outProcQ(&(sem->s_procq), p);
        if (p_out == NULL)
        {
            return NULL;
        }
        else
        {
            if (list_empty(&(sem->s_procq)))
            {
                hash_del(&(sem->s_link));
                list_add(&(sem->s_freelink), semdFree_h);
            }
            return p;
        }
    }
}

pcb_t *headBlocked(int *semAdd)
{
    semd_t *sem = getsem(semAdd);
    if (sem == NULL)
    {
        return NULL;
    }
    else
    {
        if (list_empty(&(sem->s_procq)))
        {
            return NULL;
        }
        else
        {
            pcb_t *headp = headProcQ(&(sem->s_procq));
            return headp;
        }
    }
}
void initASH()
{
    INIT_LIST_HEAD(semdFree_h);
    for (int i = 0; i < MAXPROC; i++)
    {
        list_add(&semd_table[i].s_freelink, semdFree_h);
    }
}