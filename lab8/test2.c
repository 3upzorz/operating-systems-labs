#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(void){
    char *cmd[] = {"cat", "abc", 0}; 
    pid_t pid;
    
    if((pid=fork())==-1){
        perror("fork");
    }

    if(pid > 0){
        //parent
        wait(&pid);
    }
    else{
        
        if(execvp(cmd[0],cmd) == -1){
            perror("execv");
        }

        //if(execlp("cat","cat abc") == -1){
        //    perror("execl");
        //}
    }

    return 0;
}
