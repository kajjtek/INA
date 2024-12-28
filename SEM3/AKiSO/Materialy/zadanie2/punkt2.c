#include <stdio.h>
#include <signal.h>

int main()
{
  int result = kill(1, SIGKILL);
  if(result==-1){
	printf("operacja nieudana\n");
	return 0;
  }
  printf("operacja udana\n");
  return 0;
}
