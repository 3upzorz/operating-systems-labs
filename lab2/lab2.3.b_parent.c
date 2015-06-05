#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char **argv){
    char *lsargs[1024] = {"ls","-l"};
    pid_t pid;
    int i;

    for(i=1; i<argc; i++){
        lsargs[i+1] = argv[i];
    }
    lsargs[i+1] = NULL;

    if((pid = fork()) == -1){
        perror("Fork Error: ");
    }

    if(pid > 0){
        wait(&pid);
    }else{

        if(execvp("ls", lsargs) == -1){
            perror("");
        }
    }

    return 0;
}
