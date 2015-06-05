#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
	
	int fdin;
	int fdout;
	char buffer[1024];
	int numRead;

	if(argc == 3){

		fdin = open(argv[1], O_RDONLY);
		if(fdin == -1){
			printf("unable to open input file %s : %s\n", argv[1], strerror(errno));
			return -1;
		}
		fdout = open(argv[2], O_CREAT|O_EXCL|O_WRONLY, S_IRUSR|S_IWUSR|S_IROTH|S_IWOTH);
		if(fdout == -1){
			printf("unable to create/open output file %s : %s\n", argv[2], strerror(errno));
			return -1;
		}
		numRead = read(fdin, buffer, 1024);
		while(numRead > 0){

			if(write(fdout, buffer, numRead) == -1){
				printf("unable to write file %s : %s\n", argv[2], strerror(errno));
				return -1;
			}
			numRead = read(fdin, buffer, 1024);
		}

		if(numRead == -1){
			printf("unable to read file %s : %s\n", argv[1], strerror(errno));
			return -1;
		}
		close(fdin);
		close(fdout);
	}
	else{
		printf("Expected 2 args but there was %d\n", argc - 1 );
		return -1;
	}
	return 0;
}
