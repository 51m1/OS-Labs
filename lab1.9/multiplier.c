#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//  https://www.programmingsimplified.com/c-program-multiply-matrices
//  https://www.geeksforgeeks.org/multiplication-matrix-using-pthreads

#define colz 3
#define rowz 3
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
pthread_t threads[colz] = {0};
pthread_t threadid[colz] = {0};
long* result;
long *matA;
long *matB;
long *col;
long *row;

int main(int argc, char *argv[])
{
	NUM_BUFFERS = atoi(argv[1]);
	buffers = malloc(NUM_BUFFERS*colz*sizeof(long));
	//printf("check1\n");
	matA = readMatrix("testA.dat");
	matB = readMatrix("testB.dat");
	
	multiply(matA, matB);

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
	for(i = 0; i<NUM_BUFFERS; i++)
	{
		if(pthread_mutex_trylock(&mutexes[i]))
		{
			return i;
		}
	}
	return -1;
}

int releaseLock(int lock)
{
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
		pthread_create(&threadid[i], &attr, mult_run, (void*)&threads[i]);
		printf("test2\n");
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

}

void* mult_run(void *param)
{
	int bi;
	long i;
	while(getLock()==-1);
	bi = getLock();
	for(i = 0; i < colz; i++)
	{
		buffers[bi][i] = dotProduct(getRow((int)param,matA),getColumn(i,matB));
		if((long)param == i) printf("col: %ld  %ld\n", (long)param, buffers[bi][i]);
	}
	for(i = 0; i < colz; i++)
	{
		result[(int)param*rowz+i] = buffers[bi][i];
	}
	releaseLock(bi);
	pthread_exit(0);
}

