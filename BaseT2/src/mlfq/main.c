#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //strcpy
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	char *output_name = (char *)argv[2];
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	printf("Procesos:\n");
	//Crando colas
	int Q = atoi(argv[3]);
	printf("Q = %d\n", Q );
	List* entryOrder = listInit(0, 0, 0); // Lista que almacena por startTime
	List* finishedProcesses = listInit(0, 0, 0); // Lista que almacena por startTime
	List* fifo1 = listInit(0, 0, Q * 2);
	List* fifo2 = listInit(0, 1, Q);
	List* sjf = listInit(1, 2, 0);

	

	char name[10] = "";
	int processInfo[6];
	for (int i = 0; i < input_file->len; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{	
			printf("(%d)", j);
			if (j){
				processInfo[j-1] = atoi(input_file->lines[i][j]);
				printf("%s ", input_file->lines[i][j]);
				printf("attrb: %d | ", processInfo[j-1]);
			}
			else{
				strcpy(name, input_file->lines[i][j]);
				printf("nombre = %s", name);
			}
		}
		printf("\n");
		Process* new = processInit(name, processInfo[0], processInfo[1], processInfo[2], processInfo[3], processInfo[4], processInfo[5]);
		insertSortbyStartTime(entryOrder, new);
	}

	// char subName;
	// strcpy(&subName, entryOrder -> head -> name); 
	// printf("NOMBRE == %c", subName);
	int cycleCounter = 0;
	Process* processInCPU = NULL;

	while (fifo1 -> head || fifo2 -> head || sjf -> head || entryOrder -> head || processInCPU != NULL) {
		printf("\nCiclo: %d\n", cycleCounter);

		if (processInCPU != NULL) {
			processInCPU -> sCounter ++;
			processInCPU -> cpuCounter++;
			processInCPU -> actualCpuCounter++;
			checkSFlag(processInCPU);
			if (processInCPU -> cpuCounter >= processInCPU -> cycles){
				printf("Proceso %d termino \n", processInCPU -> pid);
				processInCPU -> turnAroundTime = (cycleCounter - 1 - processInCPU -> startTime); 
				processInCPU -> waitingTime = (processInCPU -> turnAroundTime) - (processInCPU ->cpuCounter);
				processInCPU -> cpuInterruptions ++; 
				addProcess(finishedProcesses, processInCPU);
				processInCPU = NULL; 
			}
			else if (processInCPU -> cpuCounter % processInCPU -> wait == 0){
				printf("Proceso %d cede CPU \n", processInCPU -> pid);
				processInCPU -> state = 2;
				processInCPU -> waitCounter = 0;
				processInCPU -> cpuInterruptions ++; 
				if (processInCPU -> priority == 0 || processInCPU -> priority == 1) {
					addProcess(fifo1, processInCPU);
				}
				else {
					if (processInCPU -> sFlag){
						addProcess(fifo1, processInCPU);
						processInCPU -> sFlag = 0;
						processInCPU -> sCounter = processInCPU -> extraScounter;
						processInCPU -> extraScounter = 0;
						printf("S Flag durante CPU (salida x wait)\n");

					}
					else {
						addProcess(fifo2, processInCPU);
					}
				}
				processInCPU = NULL;
			}
			else if (excedesQuantum(processInCPU, Q) == 1){
				printf("Proceso %d excedio su Quantum \n", processInCPU -> pid);
				processInCPU -> cpuInterruptions ++; 
				if (processInCPU -> priority == 0) {
					addProcess(fifo2, processInCPU);
				}
				else {
					if (processInCPU -> sFlag){
						addProcess(fifo1, processInCPU);
						processInCPU -> sFlag = 0;
						processInCPU -> sCounter = processInCPU -> extraScounter;
						processInCPU -> extraScounter = 0;
						printf("S Flag durante CPU (salida quantum)\n");
					}
					else {
						addProcess(sjf, processInCPU);
					}
				}
				processInCPU = NULL;
			}
		}

		while (entryOrder -> head && entryOrder -> head -> startTime <= cycleCounter) {
			Process *enteringProcess = entryOrder -> head;
			eraseHead(entryOrder);
	 		addProcess(fifo1, enteringProcess);
		}
		
		updateProcesses(fifo1 , fifo1);
		updateProcesses(fifo2, fifo1);
		updateProcesses(sjf, fifo1);


		if (!processInCPU) {
			printf("No hay proceso\n");
			processInCPU = processReadyForExecution(fifo1);
			if (processInCPU == NULL) {
				processInCPU = processReadyForExecution(fifo2);
			}
			if (processInCPU == NULL) {
				processInCPU = processReadyForExecution(sjf);
			}
			if (processInCPU == NULL) {
				printf("No hay proceso para ejecutar");
			}
			if (processInCPU != NULL) {
				if (!processInCPU -> cpuChoice){
					processInCPU -> responseTime = (cycleCounter +1) - (processInCPU -> startTime);
				}
				processInCPU -> cpuChoice ++;
				processInCPU -> actualCpuCounter = 0;
				printf("Entra proceso desde lista %d\n", processInCPU -> priority);
			}
		}	
		cycleCounter++;
		
	}
		
	/* 
	Revisar si hay procesos que entrar al scheduler DONE

	Iterar sobre las 3 colas
		Actualizar S de procesos en colas no prioritarias.
		El S se actualiza independiente de lo que pase? o desde que el proceso sale de la cola mas prioritaria
		Si corresponde, pasar procesos en estado WAIT a Ready DONE

	Si hay un proceso en CPU revisarlo y actualizar valores.
		Hay que saber desde que cola llego a ejecutarse para saber que hacer.
		Revisar si cede la CPU. -> Se aumenta su prioridad. Si ya estaba en fifo1 se mantiene DONE
		Revisar si se acaba su quantum. -> Se baja de prioridad DONE

	Si no hay proceso en CPU, asignar un proceso a ejecutar segun prioridad. DONE
		Se reincia quantum. Si es que aplica y en funcion de que cola venia. DONE
	*/
	
	//char sentence[1000];

    // creating file pointer to work with files
    FILE *fptr;

    // opening file in writing mode
    fptr = fopen(output_name, "w");

    // exiting program 
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    int counter = 0;
	for(Process *process = finishedProcesses -> head; process; process = process -> next){
		printf("(%d)\n", counter);
		counter++;
		fprintf(fptr, "%s,%d,%d,%d,%d,%d\n", process->name, process->cpuChoice,
		 process->cpuInterruptions, process->turnAroundTime, process->responseTime, process->waitingTime);
		printf("%s, %d, %d, %d , %d, %d\n", process->name, process->cpuChoice,
		process->cpuInterruptions, process->turnAroundTime, process->responseTime, process->waitingTime);
	}
	fclose(fptr);

	if (finishedProcesses -> head) {
		Process* curr = finishedProcesses -> head -> next;
		Process* prev = finishedProcesses -> head;

		while (curr) {
			free(prev);
			prev = curr;
			curr = curr -> next;
		}
		free(prev);
		}
	free(finishedProcesses);
	free(fifo1);
	free(fifo2);
	free(entryOrder);
	free(sjf);

	input_file_destroy(input_file);
}