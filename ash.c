#include "ash.h"

static semd_t semd_table[MAXPROC];
static struct list_head semdFree_h;
DEFINE_HASHTABLE(semd_h, 10);
// aux function used to get a semaphore from the sem_h list given the key
semd_t *getsem(int *key)
{
    struct semd_t *x;
    //int j = 0;
    hash_for_each_possible(semd_h,  x, s_link, (unsigned long)key)
    {
        if (x->s_key == key)
        {
            return x; // semaphore found
        }
          // no semaphore found
    }
    return NULL;

}

int insertBlocked(int *semAdd, pcb_t *p)
{
    // Controlliamo se i parametri sono giusti
    if (semAdd == NULL || p == NULL)
        return true;
    if (p->p_semAdd != NULL)
        return true;

    semd_t *sem = getsem(semAdd);
    if (sem != NULL) // semaphore found, inserts the pcb in semprocq
    {
        list_add_tail(&p->p_list, &sem->s_procq);
        p->p_semAdd = semAdd;
        return false;
    }


    if (list_empty(&semdFree_h)) // no more free semaphores to allocate
    {
        return TRUE;
    }
    // allocates a new semaphore

    sem = list_first_entry(&semdFree_h, semd_t, s_freelink);
    list_del(&sem->s_freelink);       // deletes the first entry from the list of free semaphores
    sem->s_key = semAdd;
    INIT_LIST_HEAD(&sem->s_procq);
    list_add_tail(&p->p_list, &sem->s_procq);// inserts pcb to tail of newsem procq list
    p->p_semAdd = semAdd;
    hash_add(semd_h, &sem->s_link, (unsigned long)semAdd); // adds new sem to hashtable
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
            list_add(&(sem->s_freelink), &semdFree_h);
        }
        return pcbdel;
    }
}

pcb_t *outBlocked(pcb_t *p)
{
    semd_t *sem = container_of(p->p_semAdd, semd_t, s_link);

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
void initASH() //testata e funziona
{
    INIT_LIST_HEAD(&semdFree_h);
    for (int i = 0; i < MAXPROC; i++)
    {
        list_add(&semd_table[i].s_freelink, &semdFree_h);
    }
}