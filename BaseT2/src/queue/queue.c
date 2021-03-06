#include "queue.h"
#include "../process/process.h"
#include <stdlib.h>
#include <stdio.h>
// #include <sys/mman.h>

List* listInit(int tipo, int prioridad, int Q)
{
  List* list = calloc(1, sizeof(List));
  list-> type = tipo;
  list -> priority = prioridad;
  list -> quantum = prioridad * Q;

  return list;
}



void addProcess(List* list, Process* new){
    new -> next = NULL;
    new -> prev = NULL;
    list -> len++;
    //si es SJF insertamos ; type = 1
    if (list -> type){
      insertSortbyCyclesLeft(list, new);
    }
    //FIFO, agregamos al final.
    else{
      
      if (!list -> head) {
        list -> head = new;
      }
      else {
        list -> tail -> next = new;
        new -> prev = list -> tail;
      }
    //printf("agregado el estrecho con id = %i al padre %i\n", person -> id, person ->parent ->id);
    list -> tail = new;
    }
}

Process* getProcess(List* list, int pid){
  for(Process* nodo = list->head; nodo -> next; nodo = nodo -> next){
    if (nodo->pid == pid){
      return nodo;
    }
  }
  return NULL;
}

void removeProcess(List* list, Process* process){
    if (process -> prev && process -> next) {
      process -> next -> prev = process -> prev;
      process -> prev -> next = process -> next;
    }
    else if (process -> next) {
      // es head
      list -> head = process -> next;
      list -> head -> prev = NULL;
    }
    else if (process -> prev) {
      // es la cola
      list -> tail = process -> prev;
      list -> tail -> next = NULL;
    }
    else {
      // es el unico proceso
      list -> head = NULL;
      list -> tail = NULL;
    }
    process -> next = NULL;
    process -> prev = NULL;
}

void eraseTail(List* list){
  if (list -> head == list -> tail){
    list -> head = NULL;
    list -> tail = NULL;
  }
  else {
    Process* cola = list -> tail;
    Process* subcola = cola -> prev;

    list -> tail = subcola;
    list -> tail -> next = NULL;
  }
}

void eraseHead(List* list){
  if (list-> head -> next){
    Process* newHead = list ->head -> next;
    list -> head = newHead;
    newHead->prev = NULL;
  }
  else{
    list -> head = NULL;
    list -> tail = NULL;
  }
  list -> len--;

}

void insertSortbyStartTime(List* list, Process* new){
  printf("Entering StartTime %d\n", new-> startTime);
  int compare = new -> startTime; 
  list -> len++;
  if (list->head){
    for(Process* nodo = list->head; nodo ; nodo = nodo -> next){

      if (compare < nodo -> startTime){
        //Mejor que Head
        if (nodo -> pid == list -> head -> pid){
          //printf("mejor que head\n");
          list -> head = new;
          nodo -> prev = new;
          new -> next = nodo;
          return;
        }
        else {
          //printf("entremedio \n");
          new -> next = nodo;
          Process* originalPrev = nodo -> prev;
          originalPrev -> next = new;
          new -> prev = originalPrev;
          nodo -> prev = new;
          return;
        }

      }
  }
  //printf("a la cola\n");
  Process* exTail = list -> tail;
  list -> tail = new;
  exTail -> next = new;
  new -> prev = exTail; 
  
  }else{
    //printf("primer insrtado\n");
    list -> head = new;
    list -> tail = new;
  }

}
  



void insertSortbyCyclesLeft(List* list, Process* new){
  printf("Entering CyclesLeft %d\n", new-> cyclesLeft);
  int compare = (new -> cycles) - (new -> cpuCounter); 
  list -> len++;
  if (list->head){
    for(Process* nodo = list->head; nodo ; nodo = nodo -> next){

      if (compare < (nodo -> cycles) - (nodo -> cpuCounter)){
        //Mejor que Head
        if (nodo -> pid == list -> head -> pid){
          printf("mejor que head\n");
          list -> head = new;
          nodo -> prev = new;
          new -> next = nodo;
          return;
        }
        else {
          printf("entremedio \n");
          new -> next = nodo;
          Process* originalPrev = nodo -> prev;
          originalPrev -> next = new;
          new -> prev = originalPrev;
          nodo -> prev = new;
          return;
        }

      }
  }
  printf("a la cola\n");
  Process* exTail = list -> tail;
  list -> tail = new;
  exTail -> next = new;
  new -> prev = exTail; 
  
  }else{
    printf("primer insrtado\n");
    list -> head = new;
    list -> tail = new;
  }
}

void showList(List* list){

  printf("Imprimiendo Lista \n");
  for(Process *process = list -> head; process; process = process -> next){
    printf( "pid = %d | startTime = %d | cycles = %d | cpuCount = %d\n", process->pid, process->startTime, process->cycles, process->cpuCounter);
  }
}

void updateProcesses(List* list, List* fifo1 ){
  for(Process* process = list -> head; process  ; process = process -> next){
        process -> sCounter++;
        if (process -> state == 2) {
          process -> waitCounter ++;
        }
        if (process -> waitCounter > process -> waitingDelay) {
          process -> state = 0;
          process -> waitCounter = 0;
        }
        if (list -> priority != 0 && (process -> sCounter) % (process -> S) == 0){
          Process* newPriority = process;
          removeProcess(list, process);
          addProcess(fifo1, newPriority);
          process -> sCounter =0;
          printf("Salida  hacia fifo1, por envejecimiento\n");
          //remove de la lista actual 
        }
  }


}

Process* processReadyForExecution(List* list) {

  for(Process *process = list -> head; process; process = process -> next){
    if (process -> state == 0) {
      process -> priority = list -> priority; //al momento de ejecutarse en CPU, guardo de dde viene
      removeProcess(list, process);
      return process;
    }
  }
  return NULL;
}

int excedesQuantum(Process* process, int Q){
  if (process -> priority == 0) {
    if (process -> actualCpuCounter >= 2*Q) {
      return 1;
    }
  }
  else if (process -> priority == 1){
    if (process -> actualCpuCounter >= Q) {
      return 1;
    }
  }
  return 0;
}