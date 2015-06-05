#include <stdio.h>
#include <unistd.h>

int main(void){
	char buffer[1024];
	int numRead = read(0, buffer, 1024);
	write(1, buffer, numRead);
	return 0;
}
