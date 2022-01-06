#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char* argv[]){
	// char* xargs[MAXARG];
	// char c;
	char temp;
	// int n;
	// int i = 0;
	if(argc - 1 > MAXARG){
		printf("Too many arguments");
		exit(1);
	}
	// char* xargs[MAXARG];
	// while(i < argc - 1){
	// 	xargs[i] = argv[i+1];
	// }

	while(read(0, temp, 1) > 0){
		printf("%c", temp);
	}
	exit(0);
	// printf(buf);
	// Read input
	// Fork to create child process
	// Run exec on the command with the arguments
	// End child process and return
}

