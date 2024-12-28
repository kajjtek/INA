#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

char *lsh_line_reader()
{
 char *line = NULL; // null so that the getline knows that
		    //it needs to allocate memory dynamically
 ssize_t bufsize = 0;

 if(getline(&line, &bufsize, stdin) == -1){
     if(feof(stdin)){//the terminal works as long as eof aint received
      exit(EXIT_SUCCESS); //if we press ctrl+d ie it will be stopped
     }
     else {
      perror("readline");
      exit(EXIT_FAILURE);
     }
 }
 return line;
}
