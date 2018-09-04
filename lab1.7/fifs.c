#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct node
{
	int burst;
	int weight;
	char *name;
	struct node *link;
}*front = NULL,*rear=NULL;

void insert(int num, int prior, char *name);
struct node del();
void display();

main()
{
	int count, choice, num;
	time_t t;
	char procname[90] = "Proceso ";
	char buf[10];

	srand((unsigned)time(&t));
	
	while(count>0)
	{
		insert(rand()%20,rand()%10,strcat(procname,itoa(count,buf,10)));
		count++;
	}
	
	/*
	while(1)
	{
		printf("1 Instert\n");
		printf("2 Delete\n");
		printf("3 Display\n");
		printf("4 Exit\n");
		printf("Enter your choice\n");
		scanf("%d", &choice);
		switch(choice)
		{
			case 1:
				printf("Enter an element to insert");
				scanf("%d", &num);
				insert(num);
				break;
			case 2:
				del();
				break;
			case 3:
				display();
				break;
			case 4:
				exit(1);
			default:
				printf("Wrong Input\n");
		}
	}
	*/
}

void insert(int num, int prior, char *name)
{
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	if(temp == NULL)
	{
		printf("Overflow\n");
		return;
	}
	temp->burst=num;
	temp->weight=prior;
	temp->name=name;
	temp->link=NULL;
	if(front == NULL)
	{
		front = temp;
	}
	else
	{
		rear->link=temp;
	}
	rear = temp;
}

struct node del()
{
	//int num;
	struct node *temp;
	if(front == NULL)
	{
		printf("Underflow\n");
		return;
	}
	temp = front;
	//num = temp->burst;
	front = front->link;
	//free(temp);
	return *temp;
}

void display()
{
	struct node *p;
	if(front == NULL)
	{
		printf("Underflow\n");
		return;
	}
	p=front;
	printf("Queue Elements:\n");
	while(p != NULL)
	{
		printf("%s", p->name);
		printf("%d", p->burst);
		printf("%d", p->weight);
		p=p->link;
	}
}

