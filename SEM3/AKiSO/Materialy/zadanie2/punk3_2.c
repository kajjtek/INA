#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int signal_count =0;

void usr_handler(int dummy){
  signal_count++;
  printf("processes processed: %d\n",signal_count);
}


int main()
{

  struct sigaction sa;
  sa.sa_handler = usr_handler;
  sa.sa_flags=0;
  sigemptyset(&sa.sa_mask);

  if(sigaction(SIGUSR1, &sa, NULL)==-1){
	perror("problem z rejestracja\n");
	return 1;
  }

  printf("Process ID: %d\n", getpid());

  while(1){}
  return 0;
}
