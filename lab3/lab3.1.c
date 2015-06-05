#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){

    char *buf = malloc(BUFSIZ);
    char **ibuf; 
    int i;
    int readBytes;
    int pid;

    write(1,">",sizeof(char)); 

    while((readBytes = read(0,buf,BUFSIZ)) != 0){

        if(readBytes == -1){
            perror("");
            return -1;
        }

        i = 0;
        ibuf = malloc(BUFSIZ);
        ibuf[i] = strtok(buf," \n");
        printf("%s\n",ibuf[i]);

        while(ibuf[i] != NULL){
            ibuf[++i] = strtok(NULL," \n");
            printf("%s\n",ibuf[i]);
        }

        if((pid = fork()) == -1){
            perror("Fork error -");
            return -1;
        }
        
        if(pid > 0){
            //parent
            wait(&pid);
        }else{
            if(execvp(ibuf[0], ibuf) == -1){
                perror("exec error ");
                return -1;
            }
            return 0;
        }

        free(ibuf);
        free(buf);
        buf = malloc(BUFSIZ);
        write(1,">",sizeof(char)); 
    }

    return 0; 
}
