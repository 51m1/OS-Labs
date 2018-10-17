#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//int philosophers[5] = {0,1,2,3,4};

//void pickup_forks(int p_num);
//void return_forks(int p_num);

int main(int argc, char *argv[])
{
	
	int r,i;
	for(i=0;i<15;i++)
	{
		r = (rand()%(3+1));
		printf("Number = %d\n",r);
	}

}
