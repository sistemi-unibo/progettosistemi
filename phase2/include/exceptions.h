#include "pandos_const.h"
#include "pandos_types.h"
#include "types.h"
#include "pcb.h"
#include <umps/libumps.h>
#include "scheduler.h"
#include "syscall.h"

void exceptionHandler();
void passupordie(int index, state_t *exceptionState);
void syscallHandler(state_t *exceptionState);
/*
copies the entire state pointed by source into the state pointed by dest
*/
void copyOfState(state_t* source, state_t* dest);