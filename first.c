#include<stdio.h>
#include<pthread.h>

#define NTHREADS 10

void * job (void * arg){
	int ident = * (int *) arg;
	printf("Hi, this is the new thread %d!\n", ident);
	pthread_exit(NULL);
}

int main (void){
	pthread_t tid[NTHREADS]; //thread identifier
	int ident[NTHREADS]; //local identifier
	//create new threadS
	for(int i=1; i<=NTHREADS; ++i){
		ident[i-1] = i;
		if (pthread_create(&tid[i-1], NULL, job, (void *)&ident[i-1])){ 
			printf("ERROR -- pthread_create\n");
		}
	}
	//wait the other threads to end
	for(int i=1; i<=NTHREADS; ++i){
		if (pthread_join(tid[i-1], NULL)){ 
			printf("ERROR -- pthread_create\n");
		}
	}
	//print welcome
	printf("Hi, this is the main thread!\n");
	//separates main
	pthread_exit(NULL);
	return 0;

}
