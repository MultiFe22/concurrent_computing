#include<stdio.h>
#include<pthread.h>

void * job (void * arg){
	printf("Hi, this is the new thread!\n");
	pthread_exit(NULL);
}

int main (void){
	pthread_t tid; //thread identifier
	//create a new thread
	if (pthread_create(&tid,NULL, job, NULL)){ 
		printf("ERROR -- pthread_create\n");
	}
	//print welcome
	printf("Hi, this is the main thread!\n");
	//separates main
	pthread_exit(NULL);
	return 0;

}
