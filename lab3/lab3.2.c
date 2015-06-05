#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void printStats(int isParent){
    if(isParent == 1){
        printf("parent: ");
    }else{
        printf("child: ");
    }
    printf("pid: %d ", getpid());
    printf("ppid: %d ", getppid());
    printf("pgid: %d\n", getpgrp());
}

int main(void){
    int pid;

    if((pid = fork()) == -1){
        perror("");
        return -1;
    }
    
    if(pid > 0){
        
        wait(&pid);
        printStats(1);
    }else{
    
        printStats(0); 

        if(setpgid(0,(getpgrp() + 1)) == -1){
            perror("Set group error");
        }
        
        printStats(0);
    }

    return 0;
}
