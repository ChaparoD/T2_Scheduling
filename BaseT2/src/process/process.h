#pragma once


struct process;
typedef struct process Process;
struct process {
    int pid;
    char* name[32];
    int priority; // 1 top 
    int state;   // READY (0), RUNNING (1), , WAITING(2) o FINISHED (3).

    int startTime;
    int cycles;
    int wait;
    int waitingDelay;
    int S;

    Process *prev;
    Process *next;


    int cpuChoice; //numero de veces que fue elegido x CPU
    int cpuCounter; //numero de ciclos trabajados en CPU (A_i)
    int cpuInterruptions;  // numero de veces interrumpido x CPU (scheduler retira de CPU)
    int turnAroundTime; // (T_salida - startTime ?)
    int responseTime; // (1st cpuChoice_time  - startime)
    int waitingTime; // (ciclos_READY + ciclos_WAITING) (B_i)
    int cyclesLeft;

};




Process* processInit(char nombre, int pid, int TIEMPO_INICIO, int CYCLES, int WAIT, int WATING_DELAY, int S );