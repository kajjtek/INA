#include "parser.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "builtin.h"
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include "reader.h"
#include "pipe.h"

int lsh_launch(char **args){
  pid_t pid, wpid;
  int status;

  pid = fork();
  if(pid==0){
   if(execvp(args[0], args)==-1){
     perror("lsh");
    }
   exit(EXIT_FAILURE);
  }else if(pid < 0){
   perror("lsh");
  }else{
   do {
	wpid = waitpid(pid,&status, WUNTRACED);
  }while(!WIFEXITED(status) && !WIFSIGNALED(status));
}
 return 1;
}

int isBuiltIn(char **args){
 for(int i=0; i< lsh_builtin_num(); i++)
 {
  if(strcmp(args[0], builtin_str[i]) == 0)
  {
   return (*builtin_func[i])(args);
  }
 }
 return 0;
}

int doesExist(char **args, char *sign){
 for(int i=0; args[i]!=NULL; i++)
 {
  if(strcmp(args[i],sign)==0)
  {
   return 1;
  }
  return 0;
 }
}

int lsh_execute(char ***commands)
{
 int i,j, cmd_num=0,pipe_num=0;
 pid_t pid;

 if(commands[0] == NULL)
 {
 return 1; // empty command
 }

 for(i=0; commands[i]!=NULL; i++)
 {
  cmd_num++;
 }

 pipe_num = cmd_num-1;
 int pd_dir[pipe_num][2];

 for(i=0;i<pipe_num;i++)
 {
  if(pipe(pd_dir[i])==-1)
  {
   perror("pipe error");
   exit(EXIT_FAILURE);
  }
 }
 for(i=0;i<cmd_num;i++)
 {
  pid = fork();
  if(pid==0)
  {
   if(i>0){
    if(dup2(pd_dir[i-1][0],STDIN_FILENO)==-1)
    {
     perror("dup2 failed");
     exit(EXIT_FAILURE);
    }
   }
   if(i<pipe_num){
    if(dup2(pd_dir[i][1],STDOUT_FILENO)==-1)
    {
     perror("dup2 failed");
    }
   }
   for(j=0;commands[i][j]!=NULL;j++)
   {
    if(strcmp(commands[i][j],">")==0)
    {
    int fd_out;
     commands[i][j]=NULL;
     if((fd_out=open(commands[i][j+1], O_WRONLY|O_RDONLY|O_CREAT,0644))==-1)
     {
      perror("descriptor create problem");
      exit(EXIT_FAILURE);
     }
     dup2(fd_out,STDOUT_FILENO);
     close(fd_out);
    }
    else if(strcmp(commands[i][j],"<")==0)
    {
     int fd_in;
     commands[i][j]=NULL;
     if((fd_in=open(commands[i][j+1], O_RDONLY))==-1)
     {
      perror("descriptor create problem");
      exit(EXIT_FAILURE);
     }
     printf("%d\n", fd_in);
     if(dup2(fd_in,STDIN_FILENO)==-1){perror("dup2 failed"); exit(EXIT_FAILURE);}
     close(fd_in);
    }
    else if(strcmp(commands[i][j],"2>")==0)
    {
     int fd_err;
     commands[i][j]=NULL;
     if((fd_err=open(commands[i][j+1], O_WRONLY|O_CREAT|O_RDONLY,0644))==-1)
     {
      perror("descriptor create problem");
      exit(EXIT_FAILURE);
     }
     dup2(fd_err,STDERR_FILENO);
     close(fd_err);
    }
   }

   for(j=0;j<pipe_num;j++)
   {
    close(pd_dir[j][0]);
    close(pd_dir[j][1]);
   }

   if(isBuiltIn(commands[i])==1)
    {
     return 1;
    }

   execvp(commands[i][0],commands[i]);
   perror("execvp");
   exit(EXIT_FAILURE);
  }
  else if(pid<0)
  {
   perror("fork problem");
   exit(EXIT_FAILURE);
  }
 }
 for(j=0;j<pipe_num;j++)
 {
  close(pd_dir[j][0]);
  close(pd_dir[j][1]);
 }

 for(j=0;j<cmd_num;j++)
 {
  wait(NULL);
 }
 return 1;
}

void lsh_loop(){
  char *line;
  char **args;
  char ***commands;
  int status;

  do {
	printf("lsh > ");
	line = lsh_line_reader(); //calls line reader from "reader.h"
	args = lsh_line_splitter(line); //calls spliiter from "parser.h"
	commands = pipe_splitter(args);
	status = lsh_execute(commands); // execute the programme 

	free(line); //free function deallocates memory of given resource
	free(args);
	free(commands);
  } while(status); //as long as status aint null
}

int main(int argc, char **argv){

  lsh_loop();


  return EXIT_SUCCESS;
}
