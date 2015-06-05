#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main(void){

    pid_t pid;
    
    if((pid = fork()) == -1){
        perror("Fork Error: ");
    }
    
    if(pid > 0){
        pause();
    }else{
        sleep(3);
    }

    return 0;
}
