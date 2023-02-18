#include "ash.h"

static semd_t semd_table[MAXPROC];
static struct list_head semdFree_h;
DEFINE_HASHTABLE(semd_h, 10);

// aux function used to get a semaphore from the sem_h list given the key
semd_t *getsem(int *key)
{
    struct semd_t *x;
    hash_for_each_possible(semd_h, x, s_link, (unsigned long)key)
    {
        if (x->s_key == key)
        {
            return x; // semaphore found
        }
    }
    // no semaphore found
    return NULL;
}

int insertBlocked(int *semAdd, pcb_t *p)
{
    // Parameters check
    if (semAdd == NULL || p == NULL)
        return TRUE;
    if (p->p_semAdd != NULL)
        return TRUE;

    semd_t *sem = getsem(semAdd);
    if (sem != NULL) // semaphore found, inserts the pcb in semprocq
    {
        insertProcQ(&sem->s_procq, p);
        p->p_semAdd = semAdd;
        return FALSE;
    }
    if (list_empty(&semdFree_h)) // no more free semaphores to allocate
    {
        return TRUE;
    }
    // allocates a new semaphore
    sem = list_first_entry(&semdFree_h, semd_t, s_freelink);
    list_del(&sem->s_freelink); // deletes the first entry from the list of free semaphores
    sem->s_key = semAdd;
    INIT_LIST_HEAD(&sem->s_procq);
    insertProcQ(&sem->s_procq, p); // inserts pcb to tail of sem procq list
    p->p_semAdd = semAdd;
    hash_add(semd_h, &sem->s_link, (unsigned long)semAdd); // adds new sem to hashtable
    return FALSE;
}

pcb_t *removeBlocked(int *semAdd)
{
    if (semAdd == NULL)
    {
        return NULL;
    }

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

        pcb_t *pcbdel = removeProcQ(&(sem->s_procq));
        pcbdel->p_semAdd = NULL;
        if (list_empty(&(sem->s_procq)))
        {
            hash_del(&(sem->s_link));
            list_add(&(sem->s_freelink), &semdFree_h);
        }
        return pcbdel;
    }
}

pcb_t *outBlocked(pcb_t *p)
{
    if (p == NULL || p->p_semAdd == NULL)
    {
        return NULL;
    }

    semd_t *sem = getsem(p->p_semAdd);

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
                list_add(&(sem->s_freelink), &semdFree_h);
            }
            return p;
        }
    }
}

pcb_t *headBlocked(int *semAdd)
{
    if (semAdd == NULL)
    {
        return NULL;
    }

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
void initASH() // testata e funziona
{
    INIT_LIST_HEAD(&semdFree_h);
    for (int i = 0; i < MAXPROC; i++)
    {
        list_add(&semd_table[i].s_freelink, &semdFree_h);
    }
}