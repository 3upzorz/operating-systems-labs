#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char **argv){
    pid_t pid = fork();

    if(pid < 0){
        perror("Fork Error: ");
    }

    if(pid > 0){
        wait(&pid);
    }else{
        //TODO write exec code
        if(execv("./lab2.3.a_child", argv) == -1){
            perror("Error executing: ");
        }
    }

    return 0;
}
