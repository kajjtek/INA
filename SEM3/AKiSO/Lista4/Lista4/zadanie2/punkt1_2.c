#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void signal_handler(int dummy)
{
  printf("Custom handler\n");
}

int main()
{

  for( int i = 1; i < NSIG; i++){
		printf("Handling the signal: (%s)\n", strsignal(i));
		signal(i, signal_handler);
		raise(i);
	
  }

}


