#pragma once
#include "../process/process.h"

struct queue;
typedef struct queue List;
struct queue {
    int priority; //0 para el de más prioridad y 1 para el de menos (FIFO)
    int type; // 0 es FIFO y 1 es SJF
    int quantum;  // será == 0 en Queue SJF

    int len;
    Process* head;
    Process* tail;
};



List* listInit(int tipo, int prioridad, int Q);
void addProcess(List* list, Process* new);
Process* getProcess(List* list, int pid);
void eraseTail(List* list);
void eraseHead(List* list);

Process* processReadyForExecution(List* list);
void removeProcess(List* list, Process* process);
void updateProcesses(List* list);

void insertSortbyStartTime(List* list, Process* nodo);
void insertSortbyCyclesLeft(List* list, Process* nodo);
void showList(List* list);


  