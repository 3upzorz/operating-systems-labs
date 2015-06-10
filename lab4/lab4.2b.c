#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <string.h>

void *readFIFO(void *filename){
    
    char buf[BUFSIZ];
    ssize_t bytesRead = 0;
    int fd = open(filename, O_RDONLY);
    if(fd == -1){
        printf("Error opening file: %s\n", filename);
        return 0;
    }
    
    while(1){

        while((bytesRead = read(fd,buf,BUFSIZ)) != 0){
            if(bytesRead == -1){
                printf("Error reading file: %s\n", filename);
            }

            printf("%s = %s\n", filename, buf);
        }
    }

    return 0;
}

int main(void){

    pthread_t t1;
    pthread_t t2;
    int t1err;
    int t2err;
    
    if((t1err = pthread_create(&t1,0,readFIFO,"fifo1")) != 0){
        printf("Error creating thread: %s\n", strerror(t1err));
    }

    if((t2err = pthread_create(&t2,0,readFIFO,"fifo2")) != 0){
        printf("Error creating thread: %s\n", strerror(t1err));
    }

    if((t1err = pthread_join(t1, 0)) != 0){
        printf("Error joining thread: %s\n", strerror(t1err));
    }

    if((t2err = pthread_join(t2, 0)) != 0){
        printf("Error joining thread: %s\n", strerror(t2err));
    }
    return 0;
}
