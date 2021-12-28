
//SEMAFORI
#define NUM_SEMS 2
#define LOCK					\
	sops.sem_num = 1;			\
	sops.sem_op = -1;			\
	sops.sem_flg = 0;			\
	semop(s_id, &sops, 1);
#define UNLOCK					\
	sops.sem_num = 1;			\
	sops.sem_op = 1;			\
	sops.sem_flg = 0;			\
	semop(s_id, &sops, 1);


#define NUMERO_USER 10  
#define NUMERO_NODI 15

#define TRANSAZIONI_MAX_LIBROMASTRO 100
#define CAMPI_TRANSAZIONI 5

#define TEST_ERROR    if (errno) {dprintf(STDERR_FILENO,		\
					  "%s:%d: PID=%5d: Error %d (%s)\n", \
					  __FILE__,			\
					  __LINE__,			\
					  getpid(),			\
					  errno,			\
					  strerror(errno));}



struct libroMastroMemoria{
    int pidUser[NUMERO_USER];//semaforo SEM_USER
    /*
        letti: user(scegi a chi mandaresoldi)n
        scrittura: master/user(chi scrive tutti i pid) | quando un User viene cancellato deve togliere il suo pid
            (schifanella ci deve risponder )
    */  
    int pidNodi[NUMERO_NODI];
    /*
        letti: user/ se vogliamo il 30 si 
        scrittura:master/nodi 
    */
    char * libroMastro [5][TRANSAZIONI_MAX_LIBROMASTRO];//semaforo
    unsigned long   cur_indx ;  //indice libro mastro
};
