
#define _GNU_SOURCE  /* Per poter compilare con -std=c89 -pedantic */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "memMaster.h"

void stampa(int pid);
int main(int argc, char const *argv[]){
    int m_id, s_id;
	int cur_i, j,i ;
    int usersPids[NUMERO_USER],nodsPids[NUMERO_NODI];
	struct libroMastroMemoria * my_data;
	pid_t my_pid;
  	struct sembuf sops;
	
	/* Getting the IDs of IPC object from command line */
	m_id = atoi(argv[1]);
	s_id = atoi(argv[2]);

	/* Attach the shared memory to a pointer */
	my_data = shmat(m_id, NULL, SHM_RDONLY);
	TEST_ERROR;
	/* Wait for the "green light" by the parent */
	semop(s_id, &sops, 1);
	
    for(j=0,i=0; j<NUMERO_USER; j++) {
        if(my_data->pidUser[j]!=getpid()){
            usersPids[i]=my_data->pidUser[j];
            stampa(usersPids[i]);
            i++;
        }
    
    }
    for(j=0,i=0; j<NUMERO_NODI; j++) {
        if(my_data->pidNodi[j]!=getpid()){
            nodsPids[i]=my_data->pidNodi[j];
            stampa(nodsPids[i]);
            i++;
        }
    }


	exit(0);
}

void stampa(int pid){
    printf("PID :%d  I GET:%d",getpid(),pid);
}
