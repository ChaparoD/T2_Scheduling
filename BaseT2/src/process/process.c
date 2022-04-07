#include "process.h"
#include <stdlib.h>


Process* processInit(char nombre, int pid, int TIEMPO_INICIO, int CYCLES, int WAIT, int WATING_DELAY, int S)
{
    Process* new = calloc(1, sizeof(Process));
    new -> pid = pid;
    new -> startTime =  TIEMPO_INICIO;
    new -> cycles = CYCLES;
    new -> wait = WAIT;
    new -> waitingDelay = WATING_DELAY;
    new -> S = S;
  

  return new;
}


