#ifndef NS_H
#define NS_H

#include "types.h"
#include "pandos_types.h"
#include "list.h"
#include "pcb.h"
#include "hashtable.h"

//Function 19)
/*sets all of the free namespaces lists.*/
void initNamespaces();

//Function 20)
/*Returns the wanted namespace who's type is type (parameter)*/
nsd_t *getNamespace(pcb_t *p, int type);

//Function 21)
/*Adds ns to the namespaces of p and of all its children*/
int addNamespace(pcb_t *p, nsd_t *ns);

//Function 22)
/*Allocates a namespace of type type
from the correct list.*/
nsd_t *allocNamespace(int type);

//Function 23)
/*Frees ns and puts it back in the correct list.*/
void freeNamespace(nsd_t *ns);


#endif