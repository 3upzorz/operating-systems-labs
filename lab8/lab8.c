#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void tokenizeCmd(char *command, char **arglist){
    int i;
    char *token;

    i = 0;
    token = strtok(command," \n");

    while(token != NULL){
        arglist[i++] = token;
        token = strtok(NULL," \n");
    }
    arglist[i] = 0;
}

int main(void){

    char buf[BUFSIZ];
    int readBytes;
    char *pipeTokens[2];
    char *pipeToken;
    char *ibuf[BUFSIZ/2+1];
    int pCount;
    int fd[2];
    pid_t pid[2];
    int i;

    write(1,">",sizeof(char)); 

    while((readBytes = read(0,buf,BUFSIZ)) != 0){

        if(readBytes == -1){
            perror("");
            return -1;
        }

        pCount = 0;
        buf[readBytes] = 0;
        pipeToken = strtok(buf,"|");
        while(pipeToken != NULL){
            pipeTokens[pCount++] = pipeToken;
            pipeToken = strtok(NULL,"|");
        }

        if(pCount > 1){
            //there is a pipe in the string
            if(pipe(fd) == -1){
                perror("pipe");
            }
        }

        if((pid[0]=fork())==-1){
            perror("fork 1");
        }
        
        if(pid[0] > 0){
            //parent
            //wait for child processes
            if(pCount > 1){
                close(fd[0]);
                close(fd[1]);
            }

            wait(&pid[0]);

            if(pCount > 1){
                wait(&pid[1]);
            }
        }
        else{
        
            if((pid[1]=fork())==-1){
                perror("fork 2");
            }

            if(pid[1] > 0){
                //write to pipe
                if(pCount > 1){
                    //close read end of pipe
                    //redirect stdout to write end of pipe
                    if(close(fd[0]) == -1){
                        perror("close read end of pipe");
                    }

                    if(dup2(fd[1],1) == -1){
                        perror("dup2 output");
                    }
                    
                }
                i = 0;
            }
            else{
                //read from pipe
                if(pCount < 2){
                    //there is no pipe
                    exit(0);
                }
                //close write end of pipe
                //redirect stdin to read end of pipe

                if(close(fd[1]) == -1){
                    perror("close write end of pipe");
                }

                if(dup2(fd[0],0) == -1){
                    perror("dup2 input");
                }
                i = 1;
            }
            tokenizeCmd(pipeTokens[i],ibuf);

            if(execvp(ibuf[0], ibuf) == -1){
                perror("exec error");
            }
        }

        write(1,">",sizeof(char)); 
    }

    write(1,"\n",1);
    return 0; 
}
