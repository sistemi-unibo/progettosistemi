#include "ns.h"

static nsd_t type_nsd[MAXPROC];
static struct list_head type_nsFree_h;
static struct list_head type_nsList_h;

void initNamespaces() //testata e funziona
{
    INIT_LIST_HEAD(&type_nsFree_h);
    INIT_LIST_HEAD(&type_nsList_h);
    for (int i = 0; i < MAXPROC; i++)
    {
        list_add(&type_nsd[i].n_link, &type_nsFree_h);
    }
    
}


nsd_t *getNamespace(pcb_t *p, int type)
{
return p->namespaces[type];
}

