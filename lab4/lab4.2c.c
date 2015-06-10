#include <stdio.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define FD_SIZE 2

int main(void){

    struct pollfd fds[FD_SIZE];
    int readyCount = 0;
    int i;
    ssize_t bytesRead;
    char buf[BUFSIZ];

    if((fds[0].fd = open("fifo1", O_RDONLY)) == -1){
        perror("");
        return -1;
    }
    
    if((fds[1].fd = open("fifo2", O_RDONLY)) == -1){
        perror("");
        return -1;
    }

    fds[0].events = POLLIN;
    fds[1].events = POLLIN;

    while(1){
        readyCount = poll(fds, FD_SIZE, 1000);

        if(readyCount == -1){
            perror("poll");
            return -1;
        }else if(readyCount > 0){
            //TODO Figure out why only prints from FIFO1
            for(i = 0; i < FD_SIZE; i++){
                if(fds[i].revents & POLLIN){

                    //TODO IMPLEMENT THIS WITH THREADS TO READ FROM EACH INDIVIDUAL FILE
                    if((bytesRead = read(fds[i].fd, buf, BUFSIZ)) != 0){
                        if(bytesRead == -1){
                            perror("");
                        }else{
                            printf("Fifo%d = %s\n", i+1, buf);
                        }
                    }
                } 
            }
        }else{
            printf("poll timeout\n");
        }
    }

    return 0;
}
