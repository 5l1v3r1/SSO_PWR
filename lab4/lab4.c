#include <pthread.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 

#include <time.h>
  
pthread_cond_t cv = PTHREAD_COND_INITIALIZER; 

pthread_mutex_t mtx;

int acc1=20;
int acc2=0;
int acc3=0;

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

	usleep(rand()%1001);

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


void *fthread1(void *argp){

	pthread_mutex_lock(&mtx);

    // heading into monitor loop, we own the predicate mutex
    while (1)
    {
        // check predicate
        while (acc1<=0){
            pthread_cond_wait(&cv, &mtx);
			printf("Waiting for $ on account 1...\n");
		}

        // some state that is part of the predicate to 
        // inform us we're finished
        if (0)
            break;

        przelew(&acc1,&acc2,1);

		printf("Acc1 is %d;\nAcc2 is %d;\nAcc3 is %d.\n\n",acc1,acc2,acc3);

        // unlatch the mutex to do our predicate-independant work.
        pthread_mutex_unlock(&mtx);

		pthread_cond_signal(&cv);

        usleep(rand()%140001);

        // re-latch mutex prior to heading into wait
        pthread_mutex_lock(&mtx);            
    }

    // we still own the mutex. remember to release it on exit
    pthread_mutex_unlock(&mtx);

    return NULL;
}

void *fthread2(void *argp){

	pthread_mutex_lock(&mtx);

    // heading into monitor loop, we own the predicate mutex
    while (1)
    {
        // check predicate
        while (acc2<=0){
            pthread_cond_wait(&cv, &mtx);
			printf("Waiting for $ on account 2...\n");
		}

        // some state that is part of the predicate to 
        // inform us we're finished
        if (0)
            break;

        przelew(&acc2,&acc3,1);

		printf("Acc1 is %d;\nAcc2 is %d;\nAcc3 is %d.\n\n",acc1,acc2,acc3);

        // unlatch the mutex to do our predicate-independant work.
        pthread_mutex_unlock(&mtx);

		pthread_cond_signal(&cv);

        usleep(rand()%480001);

        // re-latch mutex prior to heading into wait
        pthread_mutex_lock(&mtx);            
    }

    // we still own the mutex. remember to release it on exit
    pthread_mutex_unlock(&mtx);

    return NULL;
}

void *fthread3(void *argp){

	pthread_mutex_lock(&mtx);

    // heading into monitor loop, we own the predicate mutex
    while (1)
    {
        // check predicate
        while (acc3<=0){
			printf("Waiting for $ on account 3...\n");
            pthread_cond_wait(&cv, &mtx);
		}

        // some state that is part of the predicate to 
        // inform us we're finished
        if (0)
            break;

        przelew(&acc3,&acc1,1);

		printf("Acc1 is %d;\nAcc2 is %d;\nAcc3 is %d.\n\n",acc1,acc2,acc3);

        // unlatch the mutex to do our predicate-independant work.
        pthread_mutex_unlock(&mtx);

		pthread_cond_signal(&cv);

        usleep(rand()%420001);

        // re-latch mutex prior to heading into wait
        pthread_mutex_lock(&mtx);            
    }

    // we still own the mutex. remember to release it on exit
    pthread_mutex_unlock(&mtx);

    return NULL;
}




int main(){

	pthread_t pt0;

	srand(time(0));

	int condition=0;

	pthread_t pt1;

	pthread_t pt2;

	pthread_create(&pt0, NULL, fthread1, NULL);

	pthread_create(&pt1, NULL, fthread2, NULL);

	pthread_create(&pt2, NULL, fthread3, NULL);

	pthread_join(pt0, NULL);

	pthread_join(pt1, NULL);

	pthread_join(pt2, NULL);


	return 0;
}

