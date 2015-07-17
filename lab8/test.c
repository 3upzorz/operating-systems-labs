#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(void){

    int fd[2];
    pid_t pid[2];
    char *cmd1[] = {"cat", "abc", 0};
    char *cmd2[] = {"tr", "a", "A", 0};
    
    if((pid[0]=fork())==-1){
        perror("fork");
    }

    if(pipe(fd)==-1){
        perror("pipe");
    }

    if(pid[0] > 0){
        //parent
        //wait for both child processes
        wait(&pid[0]);
        wait(&pid[1]);
    }else{
         if((pid[1]=fork())==-1){
            perror("fork 1");
         }

         if(pid[1]>0){
            //output
            //close pipe input
            if(close(fd[0]==-1)){
                perror("close input");
            }
            //redirect stdout to pipeout
            if(dup2(fd[1],1)==-1){
                perror("dup output");
            }

            if(execvp(cmd1[0],cmd1) == -1){
                perror("cat exec err");
            }

         }else{
            //input
            //close pipe output 
            if(close(fd[1]==-1)){
                perror("close input");
            }
            //redirect stdin to pipein 
            if(dup2(fd[0],0)==-1){
                perror("dup input");
            }

            if(execvp(cmd2[0],cmd2) == -1){
                perror("tr exec err");
            }
         }
    }

    return 0;
}
