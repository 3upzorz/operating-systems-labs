#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(void){

    char *buf = malloc(BUFSIZ);
    char **ibuf = malloc(sizeof(char *));
    char *token;
    int i;
    int readBytes;
    int pid;
    char *inputFile,*outputFile;
    inputFile = NULL;
    outputFile = NULL;

    write(1,">",sizeof(char)); 

    while((readBytes = read(0,buf,BUFSIZ)) != 0){

        if(readBytes == -1){
            perror("");
            return -1;
        }

        i = 0;

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
                    realloc(ibuf,sizeof(char *) * i);
                    ibuf[i] = token;
                }
            }
        }
        i++;
        realloc(ibuf,sizeof(char *) * i);
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
                close(0);
                if((ifd = open(inputFile, O_RDONLY)) == -1){
                    perror("Input file error");
                    return -1;
                }
                if(dup(ifd) < 0){
                    perror("Error duping input");
                }
            }

            if(outputFile != NULL){
                close(1);
                if((ofd = creat(outputFile, 0666)) == -1){
                    perror("Output file error");
                    close(ifd);
                    return -1;
                }
                if(dup(ofd) < 0){
                    perror("Error duping output");
                }
            }

            if(execvp(ibuf[0], ibuf) == -1){
                perror("exec error ");
                close(ifd);
                close(ofd);
                return -1;
            }
            close(ifd);
            close(ofd);
            return 0;
        }

        inputFile = NULL;
        outputFile = NULL;
        memset(buf,0,BUFSIZ);
        write(1,">",sizeof(char)); 
    }

    return 0; 
}
