#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
	const char ping[5] = {'p', 'i', 'n', 'g', '\0'};
	const char pong[5] = {'p', 'o', 'n', 'g', '\0'};
	char* buf[5];
	int fd[2];
	if(pipe(fd) == -1){
		printf("Failed to create pipe");
		return 1;
	}
	int pid;
	pid = fork();
	if(pid < 0){
		printf("Error with fork");
		return 2;
	}
	// Child process needs to read
	if(pid == 0){
		if(read(fd[0], buf, sizeof(buf)) == -1){
			printf("Read error");
			return 3;
		}
		printf("%d: received ping\n", getpid());
		close(fd[0]);
		if(write(fd[1], pong, sizeof(pong)) == -1){
			printf("Write error");
			return 4;
		}
		close(fd[0]);
		exit(0);
	} else {
		if(write(fd[1], ping, sizeof(ping)) == -1){
			printf("Write error in parent");
		}
		close(fd[1]);
		wait(0);
		if(read(fd[0], buf, sizeof(buf)) == -1){
			printf("Read error in parent");
		}
		printf("%d: received pong\n", getpid());
		close(fd[0]);
		exit(0);
	}

}