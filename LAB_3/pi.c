//Lab 3
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int dim; //input dimension
int nthreads; //number of threads
//thread routine
void * job(void * arg){
	long int id = (long int) arg; //thread identifier
	double *localSum;
	long int blocSize = dim/nthreads; //size of each block
	long long int ini = id * blocSize; //initial element of the block
	long int end; //last element (!computed) of the block
	localSum = (double *) calloc(1,sizeof(double));
	if (localSum == NULL){
		fprintf(stderr, "ERROR-malloc\n"); exit(1);
	}
	if(id == nthreads - 1) end = dim;
	else end = ini + blocSize;  
	for(long long int i = ini; i<end; i++){ //separates the numbers into smallers blocks, grouping the smaller ones
		if(i%2==0){
			*localSum += (double)(1.0/(1.0 + 2.0 * i));
		}
		else{
			*localSum += (double)(-1.0/(1.0 + 2.0 * i));
		}
	}

	//returns the result of the local sum
	pthread_exit((void *) localSum);
}
//main routine
int main(int argc, char *argv[])
{
	double seqPi = 0;
	double concPi = 0;
	double start, end, deltaSeq, deltaConc;
	double *returned;
	pthread_t *tid;//thread's system id
	if (argc < 3){
		fprintf(stderr, "Type: %s <array dimension> <number of threads>\n", argv[0]);
		return 1;
	}
	dim = atoll(argv[1]);
	nthreads = atoi(argv[2]);
	//sequential solution
	/*I first chose this approach because it consumes less memory than storing all the values inside an array,
	 when storing 10^9 * 8 bytes my pc couldn't take it*/
	GET_TIME(start);
	for(long long int i = 0; i<dim; i++){ 
		if(i%2==0){
			seqPi += 1.0/(1.0 + 2.0 * i);
		}
		else{
			seqPi += -1.0/(1.0 + 2.0 * i);
		}
	}
	seqPi *= 4;

	GET_TIME(end);
	deltaSeq = end-start;
	printf("Sequential pi: %.15lf\n",seqPi);
	printf("Sequential time: %lf\n",deltaSeq);
	GET_TIME(start);
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if (tid == NULL){
		fprintf(stderr, "ERROR-malloc\n");
		return 2;
	}
	//create threads
	for(long int i = 0; i<nthreads; i++){
		if(pthread_create(tid+i, NULL, job, (void *) i)){
			fprintf(stderr, "ERROR-pthread_create\n");
			return 3;
		}
	}
	//wait for the thread's end
	for(long int i = nthreads-1; i >= 0; i--){ //prioritizes the lower values to sum first to minimize the error
		if(pthread_join(*(tid+i), (void **) &returned)){
			fprintf(stderr, "ERROR-pthread_join\n");
			return 3;
		}
		//global sum
		concPi += *returned;
		free(returned);
	}
	concPi *= 4;
	GET_TIME(end);
	deltaConc = end-start;
	printf("Concurrent time: %lf\n",deltaConc);
	printf("Concurrent pi: %.15f\n",concPi);
	printf("The difference between the concurrent solution and the constant is: %.15lf \n",concPi-M_PI);
	printf("The difference between the sequential solution and the constant is: %.15lf \n",seqPi-M_PI);
	printf("Using concurrency the algorithm runs %.3lfx faster than the sequential one\n", deltaSeq/deltaConc);
	return 0;
}

//Conclui que agrupando os numeros menores e somando eles consigo uma pequena melhora de precisão, mas quase ínfima