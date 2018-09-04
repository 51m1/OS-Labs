#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/types.h>
#include <linux/wait.h>

#define maxline 80

int main(void)
{
	char* arments[maxline/2+1];
	//char args[maxline/2+1];
	//char com[maxline/2];
	char input[maxline+1] = "";
	char backup[maxline+1];

	int shouldrun = 1;
	int i = 0;
	
	while(shouldrun == 1)
	{
		printf("osh>");
		fflush(stdout);
		
		pid_t pid;
		
		fgets(input, maxline, stdin);
		printf("You typed: %s",input);
		strcpy(backup,input);
		printf("check: %s\n",backup);

		char* ampersand = strstr(input,"&");
		printf("ampersand check = %s\n",ampersand);

		//strcpy(com,strtok(input, " \0"));
		//strcpy(args,strtok(NULL, " \0"));
		//printf("command: %s and arguments: %s\n",com,args);

		arments[i] = strtok(input," &");
		printf("First value: %s\n",arments[i]);
		
		while(arments[i] != NULL)
      		{
        		arments[++i] = strtok(NULL," &");
			printf("Rest val: %s\n", arments[i]);
      		}

		pid = fork();
		
		if(pid < 0)
		{
			printf("Error\n");
		}
		else if(pid!=0)
		{
			if(ampersand == NULL)
			{
				wait();
			}
		}
		else
		{
			execvp(arments[0], arments);
			shouldrun = 0;
		}

		if(strcmp("exit\n",input) == 0)
		{
			shouldrun = 0;		
		}

		/*
		Still needed:
		1) Fork child process
		2) Child process will invoke execvp()
		3) If command did not include &, parent invoques &
		*/
	}
	return 0;
}
