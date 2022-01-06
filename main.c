#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char* argv[]){
	// declare array of file descriptors
	int fd[2];
	if(pipe(fd) == -1){
		printf("Problem creating pipe");
		return 1;
	}
	int id = fork();
	if(id < 0){
		printf("Problem forking");
		return 2;
	}

	if(id == 0){
		close(fd[0]);
		int x;
		printf("User input:\n");
		scanf("%d", &x);
		write(fd[1], &x, sizeof(int));
		close(fd[1]);
	} else {
		close(fd[1]);
		int y;
		read(fd[0], &y, sizeof(int));
		printf("Got this input: %d\n", y);
		close(fd[0]);
	}
	return 0;
}