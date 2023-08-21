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
void copyOfState(state_t* source, state_t* dest);