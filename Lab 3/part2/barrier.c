#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>


#include <sys/shm.h>
#include <sys/wait.h>

int nproc;
int *count;

int shmid_sem, shmid_cnt;
sem_t *sem;  // sem is addr to barrier, sem+1 is addr to mutex 

void init_barrier(int numproc) {
    nproc = numproc;

    shmid_sem = shmget(IPC_PRIVATE, 2 * sizeof(sem_t), IPC_CREAT | 0600);
    shmid_cnt = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    
    printf("hi");
    
    sem = (sem_t *) shmat(shmid_sem, NULL, 0);
    count = (int *) shmat(shmid_cnt, NULL, 0);

    sem_init(sem+0, 1, 0);  // barrier
    sem_init(sem+1, 1, 1);  // mutex
}

void reach_barrier() {
    // mutex for count
    sem_wait(sem+1);
    count[0]++;
    int not_shared_count = count[0];
    sem_post(sem+1);

    if (not_shared_count == nproc) {
        sem_post(sem);
    } else {
        sem_wait(sem);

        // after curr is waken, cascade to wake everyone one by one
        sem_post(sem);
    }
}

void destroy_barrier(int my_pid) {
    if(my_pid != 0) {
        // Destroy the semaphores and detach
        // and free any shared memory. Notice
        // that we explicity check that it is
        // the parent doing it.
        sem_destroy(sem+0);
        sem_destroy(sem+1);
        shmdt(count);
        shmctl(shmid_sem, IPC_RMID, 0);
        shmctl(shmid_cnt, IPC_RMID, 0);
    }
}
