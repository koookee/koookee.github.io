// Hussein El Mokdad

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "shm_com.h" // replace "csort.c" in URL with "shm_com.h" to view
#include "semun.h" // replace "csort.c" in URL with "semun.h" to view


static int set_semvalue(int semNum);
static void del_semvalue(int semNum);
static int semaphore_p(int semNum);
static int semaphore_v(int semNum);
static int sem_id; // First semaphore
static int sem_id1; // Second semaphore


int main(){
	int inDebugMode;
	int index = -1;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff; // The pointer to the struct that contains the shared varaibles
	int shmid; // The shared memory id
	pid_t pid, wait_pid; // wait_pid is for when the parent process is waiting for the children processes to finish
	int stat_val;
	
	// Setting up the shared memory
	shmid = shmget((key_t)6000, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	if (shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1) {
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	shared_stuff = (struct shared_use_st *)shared_memory;
	
	// Setting up the semaphore 1
	sem_id = semget((key_t)7000, 1, 0666 | IPC_CREAT);
	if (!set_semvalue(0)) {
			fprintf(stderr, "Failed to initialize semaphore 1\n");
			exit(EXIT_FAILURE);
	}
	
	// Setting up the semaphore 2
	sem_id1 = semget((key_t)7001, 1, 0666 | IPC_CREAT);
	if (!set_semvalue(1)) {
			fprintf(stderr, "Failed to initialize semaphore 2\n");
			exit(EXIT_FAILURE);
	}
	
	// Ask user for input 
	char letter;
	for(int c = 0; c < 7; c++){
		printf("Enter character %d: ", c + 1); fflush(stdout);
		scanf(" %c", &letter); 
		shared_stuff->arr[c] = letter;
	}
	
	// Converts all chars to lower case
	for(int i = 0; i < 7; i++){
		shared_stuff->arr[i] = tolower(shared_stuff->arr[i]);
	}
	
	char answer[10];
	printf("Would you like to use debug mode? (yes/no)\n"); fflush(stdout);
	scanf("%s",answer); 
	if(!strcmp(answer, "yes")) inDebugMode = 1;
	else inDebugMode = 0;
	
	// Creates 3 child processes
	for (int i = 0; i < 3; i++) {
		int pid = fork();
		switch(pid)
		{	
			case -1:
				perror("fork failed");
				exit(1);
			case 0:	
				if(i == 0) index = 0;
				else if(i == 1) index = 2;
				else if(i == 2) index = 4;
				int startingIndex = index;
				char temp;
				while(!shared_stuff->isSorted){
					if((index == 1 || index == 2)){
						if (!semaphore_p(0)) exit(EXIT_FAILURE);
					}
					else if((index == 3 || index == 4)){
						if (!semaphore_p(1)) exit(EXIT_FAILURE);
					}
					if(shared_stuff->arr[index] > shared_stuff->arr[index+1]){	
						if(inDebugMode) printf("Process P%d: Performed Swapping\n", i + 1); fflush(stdout);
						temp = shared_stuff->arr[index];
						shared_stuff->arr[index] = shared_stuff->arr[index + 1];
						shared_stuff->arr[index + 1] = temp;
					}
					else if(inDebugMode) printf("Process P%d: Not Swapping\n", i + 1); fflush(stdout);
					if((index == 1 || index == 2)){
						if (!semaphore_v(0)) exit(EXIT_FAILURE);
					}
					else if((index == 3 || index == 4)){
						if (!semaphore_v(1)) exit(EXIT_FAILURE);
					}
					index++;
					if(index == startingIndex + 2) index -= 2; // If it reaches the upper limit, set it back to the original value
				}
				exit(EXIT_SUCCESS);
		}
	}
	
	// Checks if array is sorted
	while(!shared_stuff->isSorted){
		int testForSorted = 1;
		for(int j = 0; j < 6; j++){
			if(shared_stuff->arr[j] > shared_stuff->arr[j + 1]){
				testForSorted = -1;
				j = 6; // Breaks out of for loop
			}
		}
		// Double checks that the array was sorted 
		if(testForSorted == 1) {
			for(int j = 0; j < 6; j++){
				if(shared_stuff->arr[j] > shared_stuff->arr[j + 1]){
					testForSorted = -1;
					j = 6; // Breaks out of for loop
				}
			}
		}
		if(testForSorted == 1) shared_stuff->isSorted = 1;
	}
	while ((wait_pid = wait(&stat_val)) > 0); // Waits until all child processes are done
	
	printf("Sorted letters: "); fflush(stdout);
	
	// Prints sorted array
	for(int j = 0; j < 7; j++){
		if(j < 6) printf("%c, ", shared_stuff->arr[j]); 
		else printf("%c \n", shared_stuff->arr[j]); 
		fflush(stdout);
	}
	
	// Detaches the shared memory and then deletes it 
	if (shmdt(shared_memory) == -1) {
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	
	del_semvalue(0);
	del_semvalue(1);
	
	exit(EXIT_SUCCESS);
}

static int set_semvalue(int semNum){
	if(semNum == 0){
		union semun sem_union;
		sem_union.val = 1;
		if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
		return(1);
	}
	if(semNum == 1){
		union semun sem_union1;
		sem_union1.val = 1;
		if (semctl(sem_id1, 0, SETVAL, sem_union1) == -1) return(0);
		return(1);
	}
}

static void del_semvalue(int semNum)
{
	if(semNum == 0){
		union semun sem_union;
		if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore 1\n");
	}
	if(semNum == 1){
		union semun sem_union1;
		if (semctl(sem_id1, 0, IPC_RMID, sem_union1) == -1)
		fprintf(stderr, "Failed to delete semaphore 2\n");
	}
}

static int semaphore_p(int semNum)
{
	if(semNum == 0){
		struct sembuf sem_b;
		sem_b.sem_num = 0;
		sem_b.sem_op = -1; 
		sem_b.sem_flg = SEM_UNDO;
		if (semop(sem_id, &sem_b, 1) == -1) {
			fprintf(stderr, "semaphore_p 1 failed\n");
			return(0);
		}
		return(1);
	}
	if(semNum == 1){
		struct sembuf sem_b1;
		sem_b1.sem_num = 0;
		sem_b1.sem_op = -1; 
		sem_b1.sem_flg = SEM_UNDO;
		if (semop(sem_id1, &sem_b1, 1) == -1) {
			fprintf(stderr, "semaphore_p 2 failed\n");
			return(0);
		}
		return(1);
	}
}

static int semaphore_v(int semNum)
{
	if(semNum == 0){
		struct sembuf sem_b;
		sem_b.sem_num = 0;
		sem_b.sem_op = 1; 
		sem_b.sem_flg = SEM_UNDO;
		if (semop(sem_id, &sem_b, 1) == -1) {
			fprintf(stderr, "semaphore_v 1 failed\n");
			return(0);
		}
		return(1);
	}
	if(semNum == 1){
		struct sembuf sem_b1;
		sem_b1.sem_num = 0;
		sem_b1.sem_op = 1; 
		sem_b1.sem_flg = SEM_UNDO;
		if (semop(sem_id1, &sem_b1, 1) == -1) {
			fprintf(stderr, "semaphore_v 2 failed\n");
			return(0);
		}
		return(1);
	}
}
