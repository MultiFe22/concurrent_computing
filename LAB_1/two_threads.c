#include <stdio.h>
#include <pthread.h>

#define NTHREADS 2
#define ARRAYSIZE 10000

int pos1, pos2 = 10000; //global variables to "sync" the threads

void * arraysum (void * array){
	int *p;
	int count = 0;
	p = (int *) array;
	printf("One thread initialized\n");
	if(*p == 0){
		while(*(p+count)!= 1 && pos1+100 < pos2) { //this condition guarantees that the thread wich started first won't iterate over the same position as the second one
			if (*(p+count) != 1){
				*(p+count) +=1;
				count +=1;
				pos1 = count; //stores the value of the position so the code can sync both threads using pos1+100 < pos2

			}
		}
		printf("Ending of first %d\n", count);
		pthread_exit(NULL);
	}
	else{
		count = ARRAYSIZE-1;
		while(*(p+count)!= 1) {
			if (*(p+count) != 1){
				*(p+count) +=1;
				count -=1;
				pos2 = count; //stores the value of the position so the code can sync both threads using pos1+100 < pos2

			}
		}
		printf("Ending of second %d\n", count);
		pthread_exit(NULL);
	}
}

int main(void){
	pthread_t tid[NTHREADS]; //system's id for threads
	int array[ARRAYSIZE]; //10000 sized array
	int check = 0; //variable to check if the array is correct
	
	//writes a 0 in every position of the array
	for (int i = 0; i < ARRAYSIZE; ++i){
		array[i] = 0;
	}

	
	//creates two threads and passes the array addres for each thread
	for (int i = 0; i < NTHREADS; ++i){
		if (pthread_create(&tid[i], NULL, arraysum, (void *)array)){ 
			printf("ERROR -- pthread_create\n");
		}
	}

	//prioritizes the two threads created above
	for(int i=1; i<=NTHREADS; ++i){
		if (pthread_join(tid[i-1], NULL)){ 
			printf("ERROR -- pthread_create\n");
		}
	}

	//checks if the array had been altered in the right way
	for (int i = 0; i < ARRAYSIZE; ++i){
		if(array[i] == 1){
			check +=1;
		}
		else if(array[i] == 0){
			printf("ERROR, I found a zero\n");
		}
		else if(array[i] == 2){
			printf("ERROR, I found a two\n");
		}
	}


	if (check == ARRAYSIZE){
		printf("Correct!\n");
	}
	pthread_exit(NULL);
	return 0;
}