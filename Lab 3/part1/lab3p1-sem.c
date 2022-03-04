#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_PROCESSES 5

/*
Key ideas:
- Create a list of (binary) semaphores as shared variables, one for each child process
- Suppose s[i] is for child process i, then it is only posted by the thread process (i-1) after child process (i-1) finishes everything
- Child process 0 is posted by parent
*/

int main() {

    int i, j, pid;

    int shmid;
    sem_t *sem;
    shmid = shmget(IPC_PRIVATE, NUM_PROCESSES * sizeof(sem_t), IPC_CREAT | 0600);
    sem = (sem_t *) shmat(shmid, NULL, 0);

    for (i=0; i<NUM_PROCESSES; i++) {
        sem_init(sem+i, 1, 0);
    }

    
    for(i=0; i<NUM_PROCESSES; i++) {
        if((pid = fork()) == 0) {
            break;
        }
    }

    if(pid == 0) {
        sem_wait(sem+i);

        printf("I am child %d\n", i);

        for(j = i*10; j<i*10 + 10; j++){
            printf("%d ", j);
            fflush(stdout);
            usleep(250000);
        }

        printf("\n\n");

        if (i < NUM_PROCESSES - 1) {
            sem_post(sem+i+1);
        }
    }
    else {
        sem_post(sem+0);

        for(i=0; i<NUM_PROCESSES; i++) 
            wait(NULL);
    }

}

