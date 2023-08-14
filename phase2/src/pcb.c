#include "include/pcb.h"

static pcb_t pcbFree_table[MAXPROC];

static struct list_head pcbFree_h;
void initPcbs()
{
    //initialisation:
    INIT_LIST_HEAD(&pcbFree_h);
    //cycles over the pcb array and inserts every element into the list
    for (int i = 0; i < MAXPROC; i++)
    {
        list_add(&pcbFree_table[i].p_list, &pcbFree_h);
    }
}

void freePcb(pcb_t *p) 
{
    list_add_tail(&(p->p_list), &pcbFree_h);
}


pcb_t *allocPcb()
{
    //checks if the free pcb's list is empty
    if (list_empty(&pcbFree_h))
    {
        return NULL;
    }
    else
    {
        //deletes the first pcb from the free pcb's list
        struct list_head *new = pcbFree_h.next;
        list_del(pcbFree_h.next);
        pcb_t *pcb = container_of(new, pcb_t, p_list);

        //sets every parameter of the pcb's struct to NULL or 0
        pcb->p_list.next = NULL;
        pcb->p_list.prev = NULL;
        pcb->p_parent = NULL;
        INIT_LIST_HEAD(&pcb->p_child);
        INIT_LIST_HEAD(&pcb->p_sib);
        pcb->p_semAdd = NULL;
        pcb->p_time = 0;
        for (int i = 0; i < NS_TYPE_MAX; i++)
        {
            pcb->namespaces[i] = NULL;
        }
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
    //checks if the pointer is null, if not, it initilizes it to null
    if (head != NULL)
    {
        INIT_LIST_HEAD(head); 
    }
}

int emptyProcQ(struct list_head *head)
{
    if (head == NULL)
    {
        return true; 
    }
    //returns true if the list pointed by head is null
    return (list_empty(head)); 
}

void insertProcQ(struct list_head *head, pcb_t *p) 
{
    if (head != NULL && p != NULL)
    {
        //inserts pcb by tail
        list_add_tail(&p->p_list, head); 
        
    } 
}

pcb_t *headProcQ(struct list_head *head) 
{
    if (head == NULL || list_empty(head))
    {
        return NULL;
    }
    else
    {
        //returns pcb pointed to by head->next
        return container_of(head->next, pcb_t, p_list);                                  
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
        //removes and returns pcb pointed to by head->next
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
        //searches for p in the list whose head is head
        list_for_each(x, head)
        {
            if (container_of(x, pcb_t, p_list) == p)
            {
                list_del(x);
                return container_of(x, pcb_t, p_list);
            }
        }
    }
    return NULL; 
}

// pcb double linked tree functions

int emptyChild(pcb_t *p) 
{
    if (p == NULL)
    {
        return true;
    }

    //if p has no children returns true, false otherwise
    return list_empty(&p->p_child); 
}

void insertChild(pcb_t *prnt, pcb_t *p)
{
    if (prnt != NULL && p != NULL)
    {
        //sets prnt as p's parent, and adds p's siblings list to the parent's children list
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
        //deletes first child and returns it
        struct list_head *tmp = (&p->p_child)->next;
        list_del((&p->p_child)->next);
        return container_of(tmp, pcb_t, p_sib);
    }
}

pcb_t *outChild(pcb_t *p) 
{
    if (p == NULL || p->p_parent == NULL )
    {
        return NULL;
    }
    else
    {
        //sets p's parent pointer to null and deletes him from the sibling's list
        p->p_parent = NULL;
        list_del(&p->p_sib);
        
        return p;
    }
}