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
 int cmd_count = 0, arg_count = 1, i;
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

int lsh_pipe(char **args)
{
 char ***commands=result.cmd;
 char **signs=result.sgn;
 int i, cmd_num=0,pipe_num;
 pid_t pid;

 for(i=0;commands[i]!=NULL;i++){cmd_num++;}
 pipe_num = cmd_num -1;
 int pd_dir[pipe_num][2];

 for(i=0; i<pipe_num + 1; i++)
 {
  if(strcmp(signs[i],"|")==0)
  {
   if(pipe(pd[2])==-1)
   {
    perror("pipe_lsh");
    exit(EXIT_FAILURE);
   }
   pid = fork();
   if(pid==0)
   {
    if(i<pipe_num)
    {
     if(dup2(pd[0],STDIN_FILENO)==-1)
     {
      perror("dup2 failed");
      exit(EXIT_FAILURE);
     }
     if(dup2(pd[1],STDOUT_FILENO)==-1)
     {
      perror("dup2 failed");
      exit(EXIT_FAILURE);
     }
     close(pd[0]);
     close(pd[1]);
     execvp(commands[i][0],commands[i]);
     perror("execvp");
     exit(EXIT_FAILURE);
    }
   }
   close(pd[0]);
   close(pd[1]);
   wait(NULL);
  }
  else if(strcmp(signs[i],">"))
  {
   if((fd = open(commands[i+1][0], O_WRONLY)==-1)
   {
    perror("lsh_redirect problem");
    exit(EXIT_FAILURE);
   }
   dup2(STDOUT_FILENO, fd);
   launch(commands[i]);
  }
  else if(strcmp(signs[i],"<"))
  {
   if((fd = open(commands[i+1][0], O_WRONLY)==-1)
   {
    perror("lsh_redirect problem");
    exit(EXIT_FAILURE);
   }
   dup2(STDIN_FILENO, fd);
   launch(commands[i]);
  }
  else if(strcmp(signs[i],"<2"))
  {
  }
  }
 }

 for(i=0;i<pipe_num;i++)
 {
   close(pd_dir[i][0]);
   close(pd_dir[i][1]);
 }

 for(i=0;i<cmd_num;i++)
 {
  wait(NULL);
 }
 fputs("\n", stderr);
 return 1;
}
