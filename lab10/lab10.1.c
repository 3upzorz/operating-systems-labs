#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

int main(void){
    int fd, size, i;
    int *nums;

    size = sizeof(int) * 20;

    fd = shm_open("/lab10a", O_CREAT|O_RDWR, 0666);

    if(fd == -1){
        perror("shm_open");
        return -1;
    }
    
    if(ftruncate(fd,size) == -1){
        perror("ftruncate");
        return -1;
    }

    nums = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    if(nums == MAP_FAILED){
        perror("mmap");
        return -1;
    }

    for(i=0; i<20;i++){
        nums[i] = (i+1)*3;
    }

    if(munmap(nums, size) == -1){
        perror("munmap");
        return -1;
    }

    if(close(fd)==-1){
        perror("close");
        return -1;
    }

    return 0;
}
