#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "pandos_const.h"
#include "pandos_types.h"
#include "types.h"
#include "pcb.h"
#include <umps/libumps.h>
#include "scheduler.h"
#include "syscall.h"

// handles the exception based on what caused it
void exceptionHandler();

// either terminates current process or passes the exception to the support struct
void passupordie(int index, state_t *exceptionState);

// handles the syscall exception based on type
void syscallHandler(state_t *exceptionState);

// copies the entire state pointed by source into the state pointed by dest
void copyOfState(state_t *source, state_t *dest);

#endif
