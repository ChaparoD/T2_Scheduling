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
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	printf("Procesos:\n");
	//Crando colas
	int Q = atoi(argv[2]);
	printf("Q = %d\n", Q);
	List* entryOrder = listInit(0, 0, Q); // Lista que almacena por startTime
	List* fifo1 = listInit(0, 2, Q);
	List* fifo2 = listInit(0, 1, Q);
	List* sjf = listInit(1, 0, Q);

	char name[10];
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
		Process* new = processInit(*name, processInfo[0], processInfo[1], processInfo[2], processInfo[3], processInfo[4], processInfo[5]);
		insertSortbyStartTime(entryOrder, new);
	}
	showList(entryOrder);

	input_file_destroy(input_file);
}