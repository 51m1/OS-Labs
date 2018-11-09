#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define cant 1000
#define binz 65536
#define pgsize 256

int dirs[cant];
int tlb[16];
char backst[binz];
char fismem[binz];
char pgtab[pgsize];

//Wrong phisical address starts at direction 23


void readbin(void)
{
	int i;
	FILE *bin;
	bin = fopen("/home/oscreader/Desktop/OS-Labs/lab5.2/BACKING_STORE.bin","rb");
	fread(&backst,sizeof(backst),1,bin);
}

int getfrombs(int index)
{
	int num;
	num = backst[index];
	printf("Read val %d\n", num);
	return num;
}

void getNum(char *filename)
{
	int i;
	FILE *dir;
	dir = fopen(filename,"r");
	printf("%s\n", filename);
	if(dir)
	{
		for(i=0 ; i<cant ; i++)
		{
			fscanf(dir,"%d",&(dirs[i]));
			//printf("%d\n", dirs[i]);
		}
		fclose(dir);
	}
	printf("Scan finished\n");
}

int getPaj(int line)
{
	int num;
	num = dirs[line];
	printf("Read address %d\n", num);
	return num;
}



int main(void)
{
	int index0, index1; 
	int vadd, padd;
	char flags;
	padd=0;
	readbin();
	getNum("/home/oscreader/Desktop/OS-Labs/lab5.2/addresses.txt");
	
	for(index0 = 0; index0 < cant; index0++)
	{
		flags=0;
		vadd = getPaj(index0);
		index1=0;
		do
		{
			if((tlb[index1] >> 8) == (vadd >> 8))
			{
				printf("checking1: tlb= %d, vadd= %d\n",(tlb[index1] >> 8),(vadd >> 8));
				flags |= 0b1;
				padd = ((tlb[index1] & 0b11111111) << 8) + (vadd & 0b11111111);
			}
			else if (index1 ==15)
			{
				flags |= 0b11;
			}
			index1++;
		}while(!(flags & 0b1));
		index1=0;
		while(flags & 0b10)
		{
			if(pgtab[index1] == (vadd >> 8))
			{
				printf("checking2: pgtab= %d, vadd= %d\n",pgtab[index1],(vadd >> 8));
				flags = 0;
				printf("checking2.5: pg= %d, v= %d\n",((pgtab[index1] & 0b11111111) << 8),vadd & 0b11111111);
				padd = ((pgtab[index1] & 0b11111111) << 8) + (vadd & 0b11111111);
			}
			else if (index1 >= pgsize)
			{
				flags = 0b100;
			}
			index1++;
		}
		index1=0;
		if(flags & 0b100)
		{
			printf("checking3: flags= %d, index0= %d, pvadd= %d\n",flags,index0, (vadd & 0b1111111100000000));
			flags=0;
			tlb[index0%16] = (vadd & 0b1111111100000000) + index0;
			pgtab[index0] = vadd >> 8;
			for(index1 = 0; index1 < pgsize; index1++)
			{
				fismem[index1] = backst[(vadd & 0b1111111100000000) + index1];
			}
		}
		printf("%d.- Virtual address: %d ",index0, vadd);
		padd = (index0 << 8) + (vadd & 0b11111111);
		printf("Physical address: %d Value: %d\n", padd, fismem[padd]);
	}

}

