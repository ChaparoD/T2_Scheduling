#include "process.h"
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>


Process* processInit(char* nombre, int pid, int TIEMPO_INICIO, int CYCLES, int WAIT, int WATING_DELAY, int S)
{
    Process* new = calloc(1, sizeof(Process));
    strcpy(new -> name, nombre);
    printf("Mi nombre de creacion es %s\n", new -> name);
    new -> pid = pid;
    new -> startTime =  TIEMPO_INICIO;
    new -> cycles = CYCLES;
    new -> wait = WAIT;
    new -> waitingDelay = WATING_DELAY;
    new -> S = S;
  
  return new;
}


void checkSFlag(Process* check){
  if (check -> sFlag){
    check -> extraScounter++;
  }
  if ((check-> sCounter) % (check -> S) == 0){
    check -> sFlag =1;
  }
  
}

