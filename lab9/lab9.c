#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char **argv){
    int fd,length,i;
    char *data;

    if(argc < 3){
        printf("please supply file name and number of characters to uppercase\n");
        return -1;
    }

    if((fd = open(argv[1], O_RDWR))==-1){
        perror("fileopen");
        return -1;
    }

    length = (int)strtol(argv[2],NULL,10);

    if((data = (char*)mmap(0,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED){
        close(fd);
        perror("mmap");
        return -1;
    }

    for(i=0;i<length;i++){
        data[i] = toupper(data[i]);
    }

    if(munmap(data,length) == -1){
        perror("munmap");
        return -1;
    }

    close(fd);
    return 0;
}
