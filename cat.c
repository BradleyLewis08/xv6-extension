#include <stdio.h>
#include <stdlib.h>

int main() {
	// infinite loop that only stops when read(file) returns 0
	// Syntax: read(0, buffer, maximum number of bytes to read)
	// Read error occurs when it returns -1
	// Syntax: write(1, buffer, maximum number of bytes to write)
	// When would a write error happen? Write returns the number of bytes it wrote
	// successfully to the file
	char buf[512];
	int n;
	for(;;){
		n = read(0, buf, sizeof buf);
		if(n == 0){
			break;
		}
		if(n < 0){
			exit(1);
		}
		if(write(1, buf, n) != n){
			exit(1);
		}
	}
}