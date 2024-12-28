#include "builtin.h"
#include <stdio.h>
#include <unistd.h>

char *builtin_str[]={
"cd",
"help",
"exit"
};

int (*builtin_func[])(char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int lsh_builtin_num()
{
 return sizeof(builtin_str)/sizeof(char*);
}

int lsh_cd(char **args)
{
  if(args[1]==NULL)
  {
   fprintf(stderr,"lsh: expected argument to \"cd\"\n");
  }
  else
  {
   if(chdir(args[1]) != 0)
    {
     perror("lsh");
    }
  }
  return 1;
}


int lsh_help(char **args)
{
 int i;
 printf("Kajetan Plewa - LSH terminal project\n");
 printf("Handles base programms, structure: command args\n");
 printf("Following programms are buint in the shell:\n");

 for(i=0;i < lsh_builtin_num(); i++)
 {
  printf("   %s\n",builtin_str[i]);
 }

 printf("Other programs under man command\n");
}

int lsh_exit(char **args)
{
 return 0;
}
