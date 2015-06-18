#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define NITEMS 100
#define NPRODUCERS 2 
#define NCONSUMERS 2

static sem_t mutex, empty, full;

struct {
    int nused;
    int next_write;
    int next_read;
    int data[NITEMS];
} buf = {0,0,0,{0}};

void *producer(void *producerNum){
    
    while(1){
        sem_wait(&full);
        sem_wait(&mutex);
        
        int i;
        i = buf.next_write;
        buf.data[buf.next_write] = (rand() % 10) + 1;
        
        buf.next_write = (buf.next_write + 1) % NITEMS;

        sem_post(&mutex);
        sem_post(&empty);
        printf("+%d: %d --> #%d\n", (int)producerNum, buf.data[i], i);
    }

    return 0;
}

void *consumer(void *consumerNum){

    while(1){
        sem_wait(&empty);
        sem_wait(&mutex);
        
        int i; 
        i = buf.next_read;
        buf.next_read = ( buf.next_read + 1) % NITEMS;

        sem_post(&mutex);
        sem_post(&full);
        
        printf("-%d: %d <-- #%d\n", (int)consumerNum, buf.data[i], i);
    }
    return 0;
}

int main(void){
    int i, terr;
    pthread_t producers[NPRODUCERS];
    pthread_t consumers[NCONSUMERS];
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, 0);
    sem_init(&full, 0, NITEMS);
    srand(time(NULL));

    //create nproducer threads
    for(i = 0; i<NPRODUCERS; i++){
        if((terr = pthread_create(&producers[i],0,producer,(void *)i)) != 0){
            printf("Error creating thread: %s\n", strerror(terr));
        }
    }

    //create nconsumer threads
    for(i = 0; i<NCONSUMERS; i++){
        if((terr = pthread_create(&consumers[i],0,consumer,(void *)i)) != 0){
            printf("Error creating thread: %s\n", strerror(terr));
        }
    }
    
    //join nproducer && consumer thread
    for(i = 0; i<NPRODUCERS; i++){
        if((terr = pthread_join(producers[i], 0)) != 0){
            printf("Error joining thread: %s\n", strerror(terr));
        }
    }

    for(i = 0; i<NCONSUMERS; i++){
        if((terr = pthread_join(consumers[i], 0)) != 0){
            printf("Error joining thread: %s\n", strerror(terr));
        }
    }
    
    return 0;
}

