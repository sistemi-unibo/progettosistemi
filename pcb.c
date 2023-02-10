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
}

int emptyProcQ(struct list_head *head)
{
}

void insertProcQ(struct list_head *head, pcb_t *p)
{
}

pcb_t *headProcQ(struct list_head *head)
{
}

pcb_t *removeProcQ(struct list_head *head)
{
}

pcb_t *outProcQ(struct list_head *head, pcb_t *p)
{
}