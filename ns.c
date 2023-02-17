#include "ns.h"

static nsd_t type_nsd[MAXPROC];
static struct list_head type_nsFree_h;
static struct list_head type_nsList_h;

void initNamespaces() //testata e funziona
{
    INIT_LIST_HEAD(&type_nsFree_h);
    for (int i = 0; i < MAXPROC; i++)
    {
        list_add(&type_nsd[i].n_link, &type_nsFree_h);
    }
    
}


nsd_t *getNamespace(pcb_t *p, int type)
{
    int flag = 0;
    nsd_t* n = NULL;

    struct list_head * new = &type_nsList_h;
    while(new != NULL && !flag)
    {
        nsd_PTR ns = container_of(new, nsd_t, n_link);
        if(ns->n_type == type) 
        {
            flag = 1;
            n = ns;
        }

    }
    return n;

}