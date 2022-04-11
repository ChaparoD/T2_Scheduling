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
    printf("(%d)\n", new->pid); //ACA SE CAE , NO DETECTA EL PROCESO.
    //si es SJF insertamos ; type = 1
    if (list -> type){
      insertSortbyCyclesLeft(list, new);
    }
    //FIFO, agregamos al final.
    else{
      printf("FIFO\n");
      
      if (list -> len == 0) {
        list -> head = new;
        printf("HEAD\n");
      }
      else {
        printf("TAIL\n");
        list -> tail -> next = new;
        new -> prev = list -> tail;
      }
    //printf("agregado el estrecho con id = %i al padre %i\n", person -> id, person ->parent ->id);
    list -> tail = new;
    }
  showList(list);
  list -> len++;
}

Process* getProcess(List* list, int pid){
  for(Process* nodo = list->head; nodo -> next; nodo = nodo -> next){
    if (nodo->pid == pid){
      return nodo;
    }
  }
  return NULL;
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

void showList(List* list){
  printf("Imprimiendo cola Largo = %d Headpid = %d \n", list ->len, list -> head -> pid);

  for(Process* nodo = list->head; nodo  ; nodo = nodo -> next){
    printf( "pid = %d | startTime = %d \n", nodo->pid, nodo->startTime);
  }
}

void updateList(List* fifo1, List* fifo2, List* sjf){
  for(Process* nodo = fifo1->head; nodo  ; nodo = nodo -> next){
        nodo -> sCounter++;

  }


}

