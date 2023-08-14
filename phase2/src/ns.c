#include "include/ns.h"

static nsd_t type_nsd[MAXPROC];
static struct list_head type_nsFree_h;
static struct list_head type_nsList_h;

void initNamespaces()
{
    //initialisation:
    INIT_LIST_HEAD(&type_nsFree_h);
    INIT_LIST_HEAD(&type_nsList_h);
    for (int i = 0; i < MAXPROC; i++)
    {
        INIT_LIST_HEAD(&type_nsd[i].n_link);
        //adds to the free list of namespaces
        list_add(&type_nsd[i].n_link, &type_nsFree_h);
    }
}

nsd_t *getNamespace(pcb_t *p, int type)
{
    return p->namespaces[type];
}

int addNamespace(pcb_t *p, nsd_t *ns)
{
    //pointer checks:
    if (p == NULL || ns == NULL)
    {
        return FALSE;
    }

    //adds namespace
    p->namespaces[ns->n_type] = ns;
    pcb_t *x;
    //iterates on every entry of the children's list
    list_for_each_entry(x, &p->p_child, p_sib)
    {
        x->namespaces[ns->n_type] = ns;
    }

    
    list_add_tail(&ns->n_link, &type_nsList_h);
    list_del(&ns->n_link);
    return TRUE;
}

nsd_t *allocNamespace(int type)
{
    //check:
    if (list_empty(&type_nsFree_h))
    {
        return NULL;
    }

    nsd_t *new; 
    new = list_first_entry(&type_nsFree_h, nsd_t, n_link);  //saving the first entry of the free namespaces list
    list_del(&new->n_link);
    new->n_type = type;

    return new;
}

void freeNamespace(nsd_t *ns)
{
    list_del(&ns->n_link);
    list_add_tail(&ns->n_link, &type_nsFree_h);
}