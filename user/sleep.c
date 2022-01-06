#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
	int duration;
	if(argc < 2){
		printf("Incorrect number of arguments");
		return 1;
	}
	duration = atoi(argv[1]);
	sleep(duration);
	exit(0);
}