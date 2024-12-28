#include "pipe.h"
#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


char*** pipe_splitter(char **args)
{
 int cmdbufsize = 10, argbufsize=10;
 char ***commands =  malloc(cmdbufsize * sizeof(char**));
 int cmd_count = 0, arg_count = 0, i;
 commands[cmd_count] = malloc(argbufsize * sizeof(char*));

 for(i=0; args[i]!=NULL; i++)
 {
  if(arg_count > argbufsize)
  {
   argbufsize+=10;
   commands[cmd_count] = realloc(commands[cmd_count], argbufsize);
  }
  if(cmd_count > cmdbufsize)
  {
   cmdbufsize+=10;
   commands = realloc(commands, cmdbufsize);
  }

  if(strcmp(args[i],"|") == 0)
  {
   commands[cmd_count][arg_count] = NULL; //koniec nullem dla przyszlych iteracji
   cmd_count++;
   arg_count=0;
   argbufsize=10;
   commands[cmd_count] = malloc(argbufsize * sizeof(char*)); 
  }
  else
  {
   commands[cmd_count][arg_count++]=args[i];
  }
 }
 commands[cmd_count][arg_count]=NULL;
 commands[cmd_count+1] = NULL;
 return commands;
}
