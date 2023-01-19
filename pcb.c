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

pcb_t headProcQ(struct list_head *head)
{
}

pcb_t *removeProcQ(struct list_head *head)
{
}

pcb_t *outProcQ(struct list_head *head, pcb_t *p)
{
}