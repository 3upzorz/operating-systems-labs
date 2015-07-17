#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int main(void){
    int fd,length,i;
    pid_t pid;
    char *data;

    if((fd = open("comm", O_RDWR|O_CREAT|O_TRUNC ,0666))==-1){
        perror("fileopen");
        return -1;
    }

    length = 16;

    if (lseek(fd, length-1, SEEK_SET) == -1){
        close(fd);
        perror("Error calling lseek() to 'stretch' the file");
        exit(EXIT_FAILURE);
    }
        
        
    if (write(fd, "", 1) == -1){
        close(fd);
        perror("Error writing last byte of the file");
        exit(EXIT_FAILURE);
    }

    if((data = (char*)mmap(0,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED){
        close(fd);
        perror("mmap");
        return -1;
    }
    
    if((pid = fork()) == -1){
        perror("fork");
        return -1;
    }else if(pid > 0){
        //parent

        wait(&pid);

        printf("Reading from parent:%s\n",data);

        if(munmap(data,length) == -1){
            perror("munmap");
            return -1;
        }

        close(fd);
    }else{
        //child
        char message[] = "Hello parent!!!";
        for(i=0;i<length;i++){
            data[i] = message[i];
        }
    }

    return 0;
}
