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

#define USER_NAME "memUser.c"
#define NODI_NAME "memNodi.c"
int main (){
    int memoriaId ,semaforiId, i,varPid;
    char memoriaId_str[3*sizeof(semaforiId)+1];
    char semaforiId_str[3*sizeof(memoriaId)+1];
    struct libroMastroMemoria * my_data;
    struct sembuf operazioni_eseguire_semafori;
    pid_t child_pid, my_pid, *user_pids,*nodi_pids;
    char * argsUser[4] = {USER_NAME};     
    char * argsNodi[4] = {NODI_NAME};     
    /*creo area di memoria condivisa*/
    memoriaId=shmget(IPC_PRIVATE,sizeof(my_data),0600);
    TEST_ERROR;
    my_data = shmat(memoriaId, NULL, 0);
    TEST_ERROR;
    my_data->cur_indx = 0;   /*puntatore alla memoria ==0*/


	/*creazione semafori*/
	semaforiId = semget(IPC_PRIVATE, NUM_SEMS, 0600);
	TEST_ERROR;
	semctl(semaforiId,0, SETVAL,1 );//semctl(id,numero_di_inizio,comando/operazione,)
	TEST_ERROR;
	
	semctl(semaforiId,0, SETVAL,1 );//semctl(id,numero_di_inizio,comando/operazione,)
	TEST_ERROR;
    sprintf(memoriaId_str, "%d", memoriaId);
	sprintf(semaforiId_str, "%d", semaforiId);
    //user
	argsUser[1] = memoriaId_str;   
	argsUser[2] = semaforiId_str;    
	argsUser[3] = NULL;
    //nodi
    argsNodi[1] = memoriaId_str;   
	argsNodi[2] = semaforiId_str;    
	argsNodi[3] = NULL;

    
	operazioni_eseguire_semafori.sem_num =0;
	operazioni_eseguire_semafori.sem_op=-1;
	operazioni_eseguire_semafori.sem_flg = 0;
	TEST_ERROR;
    
    for (i=0; i<NUMERO_USER; i++) {
		switch (varPid=fork()) {
		case -1:
			TEST_ERROR;
			break;
		case 0:
			execve(USER_NAME, argsUser, NULL); 
			TEST_ERROR;
			break;
		default:
            user_pids[i] = varPid;
			break;
		}
	}
	for (i = 0; i < NUMERO_USER; i++){
		my_data->pidUser[i]=user_pids[i];	
	}
/*
    for (i=0; i<NUMERO_NODI; i++) {
		switch (varPid= fork()) {
		case -1:
			TEST_ERROR;
			break;
		case 0:
			execve(NODI_NAME, argsNodi, NULL); 
			TEST_ERROR;
			break;
		default:
           	nodi_pids[i] = varPid;
			break;
		}
	}

    for (i = 0; i < NUMERO_NODI; i++){
		my_data->pidNodi[i]=nodi_pids[i];	
	}

	

*/
	/*
			A CHE PUNTO SIAMO 
		abbiamo scritto i pid 
		user possono leggere
	*/
	operazioni_eseguire_semafori.sem_op=1;
    shmctl(memoriaId, IPC_RMID, NULL);//mi stacco 
}