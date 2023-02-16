#ifndef NS_H
#define NS_H

#include "types.h"
#include "pandos_types.h"
#include "list.h"
#include "pcb.h"
#include "hashtable.h"

nsd_t type_nsd[MAXPROC];
list_head *type_nsFree_h;
list_head *type_nsList_h;

#endif