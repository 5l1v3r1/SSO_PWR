#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

void move(int *acc1,int *acc2,int amount,sem_t *mutex){

	sem_wait(mutex);

	*acc1=*acc1-amount;

	*acc2=*acc2+amount;

	sem_post(mutex); 
}
  
  
int main() { 

	int shm_id;

	shm_id = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);

	if (shm_id < 0) {
		
     		printf("shmget error.\n");
		
     		exit(1);
	}

	int shm_wid;

	shm_wid = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);

	if (shm_wid < 0) {
		
     		printf("shmget error.\n");
		
     		exit(1);
	}

	int mutex;

	mutex = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);

	if (mutex < 0) {
		
     		printf("shmget error.\n");
		
     		exit(1);
	}

	if(fork()==0){

		int *ptr=shmat(shm_id,NULL,0);

		int *wtr=shmat(shm_wid,NULL,0);

		sem_t *mx=shmat(mutex,NULL,0);

		*ptr=200000;

		sem_init(mx, 0, 1); 

		
		for(int i=0;i<100000;++i){

			move(ptr,wtr,1,mx);

			printf("Account 1 has %d $ and account 2 has %d $ (in child).\n",*ptr,*wtr); 
		}

		printf("Finished transferring in child.\n");
		
		sem_unlink(mx);
		
		exit(0);
	}

	int *ptr=shmat(shm_id,NULL,0);

	int *wtr=shmat(shm_wid,NULL,0);

	sem_t *mx=shmat(mutex,NULL,0);

	sleep(1); 

	for(int i=0;i<100000;++i){

		move(ptr,wtr,1,mx);

		printf("Account 1 has %d $ and account 2 has %d $ (in parent).\n",*ptr,*wtr); 
	}

	printf("Finished transferring in parent.\n");

	sem_unlink(mx);

	wait(NULL);

	sem_destroy(mx);

    return 0; 
} 
