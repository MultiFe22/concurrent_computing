//Matrix multiplication

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


float *mat; //input matrix
float *array; //input array
float *out; //output array
int nthreads;

typedef struct{
	int id; //thread element id
	int dim; //dimension of the input
} tArgs;

//thread functions

void *job(void *arg){
	tArgs *args = (tArgs*) arg;
	//printf("Thread %d\n", args->id);
	for(int i = args->id; i<args->dim; i+= nthreads)
	{
		for (int j = 0; j < args->dim; ++j)
			{
				out[i] += mat[i * (args->dim) + j] * array[j];
			}

	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int dim; //matrix dimension
	pthread_t *tid; //threads system id
	tArgs *args; //local ids for threads and dimensions
	double start, end, delta;

	GET_TIME(start);
	//reading of the parameters
	if(argc<3) 
	{
		printf("Type: %s <matrix size> <number of threads>\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);
	if (nthreads > dim) nthreads = dim; 

	//mem allocation
	mat = (float *) malloc(sizeof(float)*dim*dim);
	if (mat == NULL){printf("ERROR--malloc\n"); return 2;}
	array = (float *) malloc(sizeof(float)*dim);
	if (array == NULL){printf("ERROR--malloc\n"); return 2;}
	out = (float *) malloc(sizeof(float)*dim);
	if (out == NULL){printf("ERROR--malloc\n"); return 2;}
	
	//data structures init
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			mat[i*dim+j] = 1; //equivalent to mat[i][j]
		}
		array[i] = 1;
		out[i] = 0;
	}
	GET_TIME(end);
	delta = end - start;
	printf("Init time: %lf\n", delta);

	//multiply matrix by the array
	GET_TIME(start);
	//structures allocation
	tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	if (tid == NULL){printf("ERROR--malloc\n"); return 2;}
	args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
	if (args == NULL){printf("ERROR--malloc\n"); return 2;}

	//thread creation
	for (int i = 0; i < nthreads; ++i)
	{
		(args+i)->id = i;
		(args+i)->dim = dim;
		if(pthread_create(tid+i, NULL, job, (void *) (args+i))){
			puts("ERRO--pthread_create");
			return 3;
		}
	}

	//wait for the thread's end
	for (int i = 0; i < nthreads; i++)
	{
		pthread_join(*(tid+i), NULL);
	}
	GET_TIME(end);
	delta = end - start;
	printf("Multiplication time: %lf\n", delta);

	//show the results
	
	/*puts("Input matrix:");
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			printf("%.1f ",mat[i*dim+j] );
		}
		puts("");
	}
	puts("Input array:");
	for (int j = 0; j < dim; ++j)
	{
		printf("%.1f ", array[j]);
	}
	puts("");
	
	puts("Output array");
	for (int j = 0; j < dim; ++j)
	{
		printf("%.1f ", out[j]);
	}
	puts("");
	*/
	

	//free the memory
	GET_TIME(start);
	free(mat);
	free(array);
	free(out);
	free(args);
	free(tid);
	GET_TIME(end);
	delta = end - start;
	printf("Ending time: %lf",delta);

	return 0;
}