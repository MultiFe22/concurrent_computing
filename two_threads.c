#include <stdio.h>
#include <pthread.h>

#define NTHREADS 2
#define ARRAYSIZE 10000

int count = 0;

void * arraysum (void * array){
	int *p;
	p = (int *) array;
	printf("One thread initialized\n");
	while(count<ARRAYSIZE-1) {
		if (*(p+count) != 1){
			*(p+count) +=1;
			count +=1;
		}
	}
	pthread_exit(NULL);
}

int main(void){
	pthread_t tid[NTHREADS];
	int array[ARRAYSIZE];
	int check = 0;
	for (int i = 0; i < ARRAYSIZE; ++i){
		array[i] = 0;
	}

	for (int i = 0; i < NTHREADS; ++i){
		if (pthread_create(&tid[i], NULL, arraysum, (void *)array)){ 
			printf("ERROR -- pthread_create\n");
		}
	}

	for(int i=1; i<=NTHREADS; ++i){
		if (pthread_join(tid[i-1], NULL)){ 
			printf("ERROR -- pthread_create\n");
		}
	}

	for (int i = 0; i < ARRAYSIZE; ++i){
		if(array[i] == 1){
			check +=1;
		}
		//printf("%d\n",array[i]); 
	}
	printf("%d\n", count);
	printf("%d\n", check);

	if (check == ARRAYSIZE){
		printf("Correct!\n");
	}
	pthread_exit(NULL);
	return 0;
}