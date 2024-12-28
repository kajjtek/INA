#include <signal.h>
#include <stdio.h>

int count = 0;

int main()
{
  int pid;
  scanf("%d", &pid);
  for(int i = 0; i<30; i++){
	printf("sending ...\n");
	kill(pid,SIGUSR1);
  }

  return 0;
}
