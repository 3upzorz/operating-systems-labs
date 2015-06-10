#include <stdio.h>
#include <pthread.h>
#include <string.h>
#define LOOP_TIMES 100000

void *printMessage(void *message){
    int i = 0;
    while(i++ < LOOP_TIMES){
        printf("%s\n", message);
    }

    return 0;
}

int main(void){
    pthread_t tid1; 
    pthread_t tid2;
    int t1err;
    int t2err;

    if((t1err = pthread_create(&tid1,0,printMessage,"hello")) != 0){
        printf("Error creating thread: %s\n", strerror(t1err));
    }

    if((t2err = pthread_create(&tid2,0,printMessage,"world")) != 0){
        printf("Error creating thread: %s\n", strerror(t2err));
    }
    
    if((t1err = pthread_join(tid1, 0)) != 0){
        printf("Error joining thread: %s\n", strerror(t1err));
    }

    if((t2err = pthread_join(tid2, 0)) != 0){
        printf("Error joining thread: %s\n", strerror(t1err));
    }

    return 0;
}
