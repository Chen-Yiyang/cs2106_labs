#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_PROCESSES 5

int main() {

    int i, j, pid;

    
    // create a shared memory region
    int shmid;
    int* shm_turn;

    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    if (shmid == -1) {
        printf("Cannot create shared memory!\n");
        exit(1);
    }

    // attach it to main and initialise to 0
    shm_turn = (int*)shmat(shmid, NULL, 0);
    if (shm_turn == (int*) -1) {
        printf("Cannot attach shared memory!\n");
        exit(1);
    }
    shm_turn[0] = 0;


    for(i=0; i<NUM_PROCESSES; i++)
    {
        if((pid = fork()) == 0) {

            // attach the shared memory region to this process, before breaking
            shm_turn = (int*)shmat(shmid, NULL, 0);
            if (shm_turn == (int*) -1) {
                printf("Cannot attach shared memory!\n");
                exit(1);
            }

            break;
        }
    }

    if(pid == 0) {
        // wait if it is not my turn
        while (shm_turn[0] != i);

        printf("I am child %d\n", i);

        for(j = i*10; j<i*10 + 10; j++){
            printf("%d ", j);
            fflush(stdout);
            usleep(250000);
        }

        printf("\n\n");

        // pass turn to next process
        shm_turn[0] += 1;
    }
    else {
        for(i=0; i<NUM_PROCESSES; i++) 
            wait(NULL);
        
        shmdt((char*)shm_turn);
        shmctl( shmid, IPC_RMID, 0);
    }

}

