#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	
	setuid(0);
	execl("/bin/bash","bash_root", (char*) NULL);
	return 0;
}
