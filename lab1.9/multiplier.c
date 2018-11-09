#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//  https://www.programmingsimplified.com/c-program-multiply-matrices
//  https://www.geeksforgeeks.org/multiplication-matrix-using-pthreads

#define colz 4
#define rowz 4
#define totz colz*rowz

long* readMatrix(char *filename);
long* getColumn(int col, long *matrix);
long* getRow(int row, long *matrix);
long dotProduct(long *vec1, long *vec2);
int getLock();
int releaseLock(int lock);
long* multiply(long *matA, long *matB);
int saveResultMatrix(long *result);
void* mult_run(void *param);

int NUM_BUFFERS;
long **buffers;
pthread_mutex_t *mutexes;
pthread_t threadid[colz] = {0};
long *result;
long *matA;
long *matB;
long *col;
long *row;

int main(int argc, char *argv[])
{
	NUM_BUFFERS = atoi(argv[1]);
	int x;
	buffers = malloc(NUM_BUFFERS*sizeof(long));
	for(x = 0; x<NUM_BUFFERS; x++)
	{
		buffers[x] = malloc(colz*sizeof(long));
	}
	mutexes = malloc(NUM_BUFFERS*sizeof(pthread_mutex_t));
	result = malloc(totz*sizeof(long));
	//printf("check1\n");
	matA = readMatrix("testA.dat");
	matB = readMatrix("testB.dat");
	//saveResultMatrix(matA);
	//printf("Result: %ld\n",dotProduct(getRow(1,matA),getColumn(1,matB)));
	//printf("Result: %ld\n",dotProduct(getRow(1,matA),getColumn(2,matB)));
	//printf("Result: %ld\n",dotProduct(getRow(1,matA),getColumn(3,matB)));	

	multiply(matA, matB);
	saveResultMatrix(result);

	/*
	int ack;
	for(ack = 0;ack<rowz;ack++)
	{
		printf("Printing col: %ld\n",row[ack]);
	}
	*/
	
	free(matA);
	free(matB);
	free(col);
	free(row);
	free(result);
	free(buffers);
}

long* readMatrix(char *filename)
{
	long *fmat;
	fmat = malloc(totz*sizeof(long));
	long i;
	FILE *mat;
	mat = fopen(filename,"r");
	printf("%s\n", filename);
	if(mat)
	{
		for(i=0 ; i<totz ; i++)
		{
			fscanf(mat,"%ld",&fmat[i]);
		}
		fclose(mat);
	}
	return fmat;
}

long* getColumn(int col, long *matrix)
{
	long *colm;
	if(col <= colz && col>0)
	{
		colm = malloc(colz*sizeof(long));
		long i;
		for(i = 0 ; i<colz ; i++)
		{
			colm[i] = matrix[col-1+i*colz];
		}
	}
	return colm;
}

long* getRow(int row, long *matrix)
{
	long *rowm;
	if(row <= rowz && row>0)
	{
		rowm = malloc(rowz*sizeof(long));
		long i;
		for(i = 0 ; i<rowz ; i++)
		{
			rowm[i] = matrix[i+(row-1)*rowz];
		}
	}
	return rowm;
}

long dotProduct(long *vec1, long *vec2)
{
	long res = 0;
	long i;
	for(i = 0 ; i<colz ; i++)
	{
		res = res + vec1[i]*vec2[i];
	}
	return res;
}

int getLock()
{
	int i;
	printf("Entered getlock()\n");
	for(i = 0; i<NUM_BUFFERS; i++)
	{
	printf("test for lock in buffer %d\n",i);
		if(pthread_mutex_trylock(&mutexes[i])==0)
		{
			printf("Assigned lock %d\n",i);
			return i;
		}
	}
	return -1;
}

int releaseLock(int lock)
{	
	printf("Released lock: %d\n",lock);
	if(pthread_mutex_unlock(&mutexes[lock]))
	{
		return 0;
	}
	return -1;
}

long* multiply(long *matA, long *matB)
{
	int i;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	printf("test1\n");
	for(i = 0; i < colz; i++)
	{
		pthread_create(&threadid[i], &attr, mult_run, (void*)i);
		printf("test2\n");
		printf("Created thread %d\n",i);
	}
	printf("test3\n");
	for(i = 0; i < colz; i++)
	{
		printf("test4\n");
		pthread_join(threadid[i], NULL);
		printf("test5\n");
	}
	return result;
}

int saveResultMatrix(long *result)
{
	long i;
	FILE *out = fopen("result.dat","w");
	for(i = 0; i < totz; i++)
	{
		fprintf(out,"%ld\n",result[i]);
	}
	fclose(out);
	return 0;
}

void* mult_run(void *param)
{
	printf("Started with thread run\n");
	int bi;
	long i;
	printf("Request a buffer/lock\n");
	while(bi = getLock()==-1);
	printf("Lock successfull with lock %d\n",bi);
	for(i = 1; i <= colz; i++)
	{
		printf("Begin multiplication with counter %ld and param %d\n",i, 			(int)param);
		buffers[bi][i-1] = dotProduct(getRow((int)param+1,matA),getColumn(i,matB));
		printf("Multiplication compleated, result: %ld\n",dotProduct(getRow	 			((int)param+1,matA),getColumn(i,matB)));
		printf("Multiplication saved, result: %ld\n",buffers[bi][i-1]);
	}
	printf("test11 totz = %d\n", totz);
	//memcpy(&result[(int)param*colz], &buffers[bi],colz*sizeof(long));
	
	for(i = 0; i < colz; i++)
	{
		printf("test12 param= %d i= %ld\n", (int)param),i;
		result[(int)param*colz+i] = buffers[bi][i];
		printf("Wrote %ld in result from buffers %ld\n",result[(int)param*colz  		+i],buffers[bi][i]);
	}
	
	printf("Finished operations in thread %d\n",(int)param);
	releaseLock(bi);
	pthread_exit(0);
}

