#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LSH_BUFFORSIZE 64
#define LSH_DELIMITERS " \t\r\n\a"

char **lsh_line_splitter(char *line){
 int bufsize = LSH_BUFFORSIZE, position = 0;
 char **tokens = malloc(bufsize * sizeof(char*));
 char *token;

 if(!tokens) { // checking whether memorry was alocated - if null then false
  fprintf(stderr, "lsh: allocation error\n");
  exit(EXIT_FAILURE);
 }

 token = strtok(line, LSH_DELIMITERS); //pointer to first character - line
 //strtok writes to token address the first character from line w/o delimiter

 while(token!=NULL) {
	tokens[position]=token;
	position++;//now we need to check if we wont cross the buffor

	if(position>=bufsize){
	 bufsize+=LSH_BUFFORSIZE;
	 tokens=realloc(tokens, bufsize * sizeof(char*)); // allocating necessary memory for char*
	 if(!tokens){
		  fprintf(stderr, "lsh: allocation error\n");
		  exit(EXIT_FAILURE);
		}
	}

	token=strtok(NULL, LSH_DELIMITERS);//if we call null the strtok will start where it left off
    }
 tokens[position] = NULL;
 return tokens;
}
