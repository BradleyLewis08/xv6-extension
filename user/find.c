#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
//   static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  return p;
}

void find(char* dir, char* query){
	struct dirent de;
	char buf [512], *p;
	int fd;
	struct stat st;


	// Create file descriptor at current directory level
	if((fd = open(dir, 0)) < 0){
		printf("Cannot open path: %s", dir);
		return;
	}

	if(fstat(fd, &st) < 0){
		fprintf(2, "ls: cannot stat %s\n", dir);
		close(fd);
		return;
	}

	switch(st.type){
		// If stat is a file, check file name and compare to query
		case T_FILE:
			if(strcmp(fmtname(dir), query) == 0){
				printf("%s\n", dir);
			}
			break;
		case T_DIR:
			// Iterate through directory of files
		    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
				printf("path too long\n");
				break;
			}
			strcpy(buf, dir);

			p = buf+strlen(buf);
			*p++ = '/';

			while(read(fd, &de, sizeof(de)) == sizeof (de)){
				if(de.inum == 0){
					continue;
				}
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;

				if(stat(buf, &st) < 0){
					printf("Failure to stat");
					continue;
				}
				switch(st.type){
					// Compare name if file
					case T_FILE:
						if(strcmp(fmtname(buf), query) == 0){
							printf("%s\n", buf);
						} 
						break;
					case T_DIR:
						// Don't recurse if directory is root/previous dir
						if(strcmp(fmtname(buf), ".") == 0 || strcmp(fmtname(buf), "..") == 0){
							continue;
						}
						// Recurse into directory
						find(buf, query);
				}
			}
			break;
    }
	return;
	close(fd);
}


int main(int argc, char* argv[]){
	if(argc < 3){
		printf("Usage: find dir pattern");
		return 1;
	}
	find(argv[1], argv[2]);
	exit(0);
}