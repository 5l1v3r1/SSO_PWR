#include <pthread.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
  
pthread_cond_t zm = PTHREAD_COND_INITIALIZER; 

pthread_mutex_t mtx;

int acc1=4000;

int acc2=0;

/*

thread:
    initialise.
    lock mutex.
    while thread not told to stop working:
        wait on condvar using mutex.
        if work is available to be done:
            copy work to thread local storage.
            unlock mutex.
            do the work.
            lock mutex.
    unlock mutex.
    clean up.
    exit thread.
*/



void wplata(int *a1,int howMuch){

	int bmp=*a1;

	usleep(1000);

	*a1=bmp+howMuch;
}


void przelew(int *a1,int *a2,int howMuch){

	if(*a1<howMuch){

		printf("Account 1 balance is too low.\n");

		return;
	}

	wplata(a1,-howMuch);

	wplata(a2,howMuch);
}


void *fthreadx(void *argp){

	pthread_mutex_lock(&mtx);

	for(int i=0;i<2000;++i){

		przelew(&acc1,&acc2,1);

		printf("Acc1 is %d; acc2 is %d.\n",acc1,acc2);
	}	

	pthread_mutex_unlock(&mtx);

	return NULL;
}


int main(){

	pthread_t pt0;

	pthread_t pt1;

	pthread_create(&pt1, NULL, fthreadx, NULL);

	pthread_join(pt0, NULL);

	pthread_join(pt1, NULL);


	return 0;
}

