#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(void){

    char buf[BUFSIZ];
    char *token;
    char *ibuf[BUFSIZ/2+1];
    int i;
    int readBytes;
    int pid;
    char *inputFile,*outputFile;

    write(1,">",sizeof(char)); 

    while((readBytes = read(0,buf,BUFSIZ)) != 0){

        if(readBytes == -1){
            perror("");
            return -1;
        }

        inputFile = NULL;
        outputFile = NULL;
        i = 0;
        buf[readBytes] = 0;
        token = strtok(buf," \n");
        ibuf[i] = token;

        while(token != NULL){

            token = strtok(NULL, " \n");

            if(token != NULL){
                if(*token == '<'){
                    token = strtok(NULL, " \n");
                    inputFile = token;
                }else if(*token == '>'){
                    token = strtok(NULL, " \n");
                    outputFile = token;
                }else{

                    i++;
                    ibuf[i] = token;
                }
            }
        }
        i++;
        ibuf[i] = 0;

        if((pid = fork()) == -1){
            perror("Fork error");
            return -1;
        }

        if(pid > 0){
            //parent
            wait(&pid);
        }else{
            int ifd,ofd;
            if(inputFile != NULL){ 
                if((ifd = open(inputFile, O_RDONLY)) == -1){
                    perror("Input file error");
                    return -1;
                }
                if(dup2(ifd, 0) < 0){
                    perror("Error duping input");
                }
                close(ifd);
            }

            if(outputFile != NULL){
                if((ofd = creat(outputFile, 0666)) == -1){
                    perror("Output file error");
                    close(ifd);
                    return -1;
                }
                if(dup2(ofd, 1) < 0){
                    perror("Error duping output");
                }
                close(ofd);
            }

            if(execvp(ibuf[0], ibuf) == -1){
                perror("exec error ");
                close(ifd);
                close(ofd);
                return -1;
            }
            return 0;
        }

        write(1,">",sizeof(char)); 
    }

    return 0; 
}
