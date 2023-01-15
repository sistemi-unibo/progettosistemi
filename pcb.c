#include "header/pcb.h"

void initPcbs() {}
void freePcb(pcb_t *p) {}
pcb_t *allocPcb() {}

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