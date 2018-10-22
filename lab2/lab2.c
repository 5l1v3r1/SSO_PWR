#include <unistd.h>
#include <stdio.h>
#include<stdlib.h> 
#include<sys/wait.h>

int main(int argc,char **argv){

	int fid[2];

	if(pipe(fid)==-1){

		printf("Blad pipe'a.\n");

		return -1;
	}


	int pid=fork();

	if(pid<0){

		printf("Blad fork'a.\n");

		return -1;
	}


	if(pid>0){

		char buff[]="Messages:\n";

		char num_str[8]={0};

		close(fid[0]);

		write(fid[1],buff,sizeof(buff));

		for(int i=0;i<=2048;++i){

			sprintf(num_str,"#%d\n",i);

			write(fid[1],num_str,sizeof(num_str));
		}

		close(fid[1]);

		wait(0);

		exit(0);
	}


	char rec_buff[128];

	close(fid[1]);

	int num_read;

	do{

		num_read=read(fid[0],rec_buff,128);

		write(1,rec_buff,num_read);

		sleep(1);

	}while(num_read>0);

	return 0;
}


