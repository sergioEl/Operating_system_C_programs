
#include <string.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * COMP2004 ass4 Q4
 * Seokho Han 201761541
 * seokhoh@mun.ca
 * pipe sending within two processes
*/

int main()
{
    pid_t pid;
    //declaring an array of characters with size 4096 
  
    char buffer[1024];
    //fdOne[0] : reading
    //fdOne[1] : writing
    //declaring variable for storing the two ends of first pipe
    int fdOne[2];
    //declaring variable for storing the two ends of second pipe 
    int fdTwo[2];
    //pipe() system call is used for passing information from one process to another
    pipe(fdOne);
    if (pipe(fdOne)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    pipe(fdTwo);
    if (pipe(fdTwo)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    }
    //forking the process
    pid = fork();
     if (pid < 0) 
    { 
        fprintf(stderr, "Fork Failed" ); 
        return 1; 
    } 
    // fork returns to parent the pid of the child
    if (pid > 0) 
    {
        int i;
        // Close writing end of first pipe
        close(fdOne[1]); 
        close(fdTwo[0]);
        //read from the pipe 
        read(fdOne[0], buffer, sizeof(buffer));
        // printf("Yes\n\n");
        for(i = 0; i<sizeof(buffer); i++) {
            if(islower(buffer[i]))
                buffer[i] = toupper(buffer[i]);
            else
                buffer[i] = tolower(buffer[i]);
        }
        write(fdTwo[1], buffer, strlen(buffer) + 1);
       // Wait for child to send a string 
        wait(NULL);

    // child process
    }else if (pid == 0){
        // Close reading end of first pipe 
        close(fdOne[0]);
        close(fdTwo[1]); 

        //ask user input 
        printf("input : ");
        fgets(buffer, sizeof(buffer), stdin);
        printf("The original message is %s", buffer);

        //write to the pipe 
        write(fdOne[1], buffer, strlen(buffer) + 1);
        read(fdTwo[0], buffer, sizeof(buffer));
        printf("The inverted message is %s",buffer);

        exit(0);
    }

    return 0;
}