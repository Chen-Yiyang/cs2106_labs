#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {

    printf("Be patient, the program will take around 7 seconds to run.\n");
    printf("At the end you can do \"cat results.out\" to see the result.\n");

    //
    // Add code here to pipe from ./slow 5 to ./talk and redirect
    // output of ./talk to results.out
    // I.e. your program should do the equivalent of ./slow 5 | talk > results.out
    // WITHOUT using | and > from the shell.
    //

    /* Ideas:
     - parent can wait for child, so child is slow and parent is talk
     - child write to pipe, parent read from pipe
     - need one more overarching process managing everything (e.g. in the case of part 2e)
     - remember to redirect final outputs
    */

    int fp_out = open("./results.out", O_CREAT | O_WRONLY);

    if(fork() == 0) {
        dup2(fp_out, STDOUT_FILENO);

        // pipe creation
        // p[0] is the reading end, p[1] is the writing end.
        int p[2];
        
        if(pipe(p) < 0) {
            perror("lab2p2f: ");
        }
        
        // We will send a message from father to child
        if(fork() != 0) { // parent is TALK
            close(p[1]);

            wait(NULL); // let child write first
            
            dup2(p[0], STDIN_FILENO);
            dup2(fp_out, STDOUT_FILENO);
            
            execlp("./talk", "talk", (char *) 0);
            
            
            close(p[0]);
            
        }
        else { // child is SLOW
            close(p[0]);
            dup2(p[1], STDOUT_FILENO);

            execlp("./slow", "slow", "5", (char *) 0);

            close(p[1]);
        }
    }
    else
        wait(NULL);

}

