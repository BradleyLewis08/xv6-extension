#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MAXARGLEN 32
char whitespace[] = " \t\r\n\v";

int main(int argc, char* argv[]){
	char* xargv[MAXARG * MAXARGLEN];
	int xargc = 0;

	// Index for supplied command line args
	int j = 1;

	// Character pointers to get command line arguments
	char* p;
	char* s;

	// Buffer for input lines
	static char buf[MAXARG * MAXARGLEN];
	int nbuf = sizeof(buf);

	if(argc - 1 > MAXARG){
		printf("Too many arguments");
		exit(1);
	}

	// Copy argv arguments into new xargv array
	while(j < argc){
		xargv[xargc] = argv[j];
		xargc++;
		j++;
	}

	// Read stdin into buffer
	for(;;){
		gets(buf, nbuf);

		if(buf[0] == 0){ // EOF
			break;
		}

		// Charcter pointer init
		p = buf;
		s = buf;

		for(;;){
			// Look for whitespace delimiter
			if(strchr(whitespace, *p)){
				// Null terminate each argument by replacing space with null character
				*p = '\0';
				// Add argument to new argv array
				xargv[xargc] = s;
				// Increment p and set s to p
				s = ++p;
				xargc++;
			} else if(*p == '\0'){ // End of input line
				// gets keeps the newline character and adds a null termination at the end
				// replace newline with null character to terminate last argument
				*--p = '\0';
				xargv[xargc] = s;
				xargc++;
				break;
			} else {
				p++;
			} 
		}
		if(fork() == 0){
			// Exec command with xargv array
			if(exec(xargv[0], xargv) == -1){
				printf("Could not exec %s\n", xargv[0]);
				exit(1);
			};
		} else {
			wait(0);
			// Reset xargc to allow for reading of next line
			xargc = j - 1;
		}
	}

	exit(0);
}

