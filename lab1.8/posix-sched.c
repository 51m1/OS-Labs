/**
 * A simple pthread program illustrating POSIX scheduling.
 *
 * Figure 6.8
 *
 * To compile:
 *
 *	gcc posix-sched.c -o posix-sched -lpthread
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013.
 */

#include <pthread.h>
#include <stdio.h>
//#define NUM_THREADS 5

/* the thread runs in this function */

void *runner(void *param); 

main(int argc, char *argv[])
{
	int param = atoi(argv[1]);
	printf("Param = %d\n",param);
	int NUM_THREADS = param;
	int i, o, scope;
	int par[2];
	pthread_t tid[NUM_THREADS]; 	/* the thread identifier */
	pthread_attr_t attr; 		/* set of attributes for the thread */

	/* get the default attributes */
	pthread_attr_init(&attr);

	/* first inquire on the current scope */
	if (pthread_attr_getscope(&attr,&scope) != 0)
		fprintf(stderr, "Unable to get scheduling scope.\n");
	else {
		if (scope == PTHREAD_SCOPE_PROCESS)
			printf("PTHREAD_SCOPE_PROCESS\n");
		else if (scope == PTHREAD_SCOPE_SYSTEM)
			printf("PTHREAD_SCOPE_SYSTEM\n");
		else 
			fprintf(stderr,"Illegal scope value.\n");
	}
	
	/* set the scheduling algorithm to PCS or SCS */
	if (pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM) != 0)
		printf("unable to set scheduling policy.\n");

	/* create the threads */
	for (i = 0; i < NUM_THREADS; i++)
	{
		o = i+4;
		par[0] = i;
		par[1] = o;
		printf("I am thread 1. Coreated new thread (%d) in iteration %d...\n",o,i);
		pthread_create(&tid[i],&attr,runner,/*NULL*//*(void *)o*/(void *)par); 
	}
	/**
	 * Now join on each thread
	 */
	for (i = 0; i < NUM_THREADS; i++) 
		pthread_join(tid[i], NULL);
}

/**
 * The thread will begin control in this function.
 */
void *runner(void *param) 
{
	/* do some work ... */
	int *tid1;
	int t1,t2;
	tid1 = (int *)param;	
	t1 = tid1[0];
	t2 = tid1[1];

	//printf("First value for tid %ld\n",(int)param);

	long tid;
	tid = (long)param;
	printf("Hello from thread #%d - I was created in iteration %d\n",t1,t2);

	pthread_exit(0);
}

