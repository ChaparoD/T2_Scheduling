#pragma once


struct process;
typedef struct process Process;
struct process {
    int pid;
    char name[10];
    int priority; // si viene de fifo1: 0, fifo2: 1, sjf: 2 
    int state;   // READY (0), RUNNING (1), , WAITING(2) o FINISHED (3).

    int startTime;
    int cycles;
    int wait;
    int waitingDelay;
    int S;

    Process *prev;
    Process *next;


    int cpuChoice; //numero de veces que fue elegido x CPU                                      (check)
    int cpuCounter; //numero de ciclos trabajados en CPU (A_i)                                  (check)
    int actualCpuCounter; //numero de ciclos trabajados en CPU desde que entro a CPU
    int cpuInterruptions;  // numero de veces interrumpido x CPU (scheduler retira de CPU)      (check)
    int turnAroundTime; // (T_salida - startTime ?)                                             (check)              
    int responseTime; // (1st cpuChoice_time  - startime)                                       (check)
    int waitingTime; // (ciclos_READY + ciclos_WAITING) (B_i)(turnAround - cpuCounter)          (check)
    int cyclesLeft; //(cycles - cpucounter)
    int sCounter; // lleva el numero de ciclos de envejecimiento.
    int waitCounter; // cuenta la cantidad de ciclos del wait actual
    int sFlag; // 1 si cumple envejecimiento durante ejecucion de Cpu
    int extraScounter; // cuenta los ciclos de envejecimiento extras acumulados si estaba en CPU

};




Process* processInit(char* nombre, int pid, int TIEMPO_INICIO, int CYCLES, int WAIT, int WATING_DELAY, int S );
void checkSFlag(Process* checking);
