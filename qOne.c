#include <stdio.h>
#include <stdlib.h>
#include "qOne.h"

#define ADD_IDENT 0
#define MULT_IDENT 1

//Note to self: a=8 ; b=4; p=17
int main(int argc, char*argv[])
{
	if(argc != 4)
	{
		printf("Usage: %s field A B", argv[0]);
	} else
	{
		int prime, A, B;
		prime = atoi(argv[1]);
		A = atoi(argv[2]);
		B = atoi(argv[3]);

		int** mTable = createMultiplicationTable(prime);
		int* invmTable = createInverseTable(mTable, prime, MULT_IDENT);
		int** rootTable = createRootTable(mTable, prime);
		int** aTable = createAdditionTable(prime);
		int* invaTable = createInverseTable(aTable, prime, ADD_IDENT);

		printmTable(mTable, prime);
		printRootTable(rootTable, prime);
		printf("\n");
		printf("Order of elliptic: %i", getOrderOfElliptic(prime, A, B, mTable, aTable, rootTable));
	}
}


void printmTable(int** table, int prime)
{
	printf("\t");
	for(int j = 1; j < prime; j++)
	{
		printf("%i\t", j);
	}
	printf("\n");
	for(int i = 1; i < prime; i++)
	{
		printf("%i\t", i);
		for(int j = 1; j < prime; j++)
		{
			printf("%i\t", table[i][j]);
		}
		printf("\n");
	}
}

void printRootTable(int** table, int prime)
{
	printf("val\troot 1\troot 2\n");
	for(int i = 1; i < prime; i++)
	{
		printf("%i\t%i\t%i\n", i, table[i][0], table[i][1]);
	}
}

int** createMultiplicationTable(int prime)
{
	
	int** table = (int**) malloc(prime * sizeof(int*));

	for(int i = 1; i < prime; i++)
	{
		table[i] = (int*) malloc(prime * sizeof(int));
		
		for(int j = 1; j < prime; j++)
		{
			int val = i * j;
			table[i][j] = val % prime;
		}
	}
	return table;
}

int* createInverseTable(int** table, int prime, int ident)
{
	int* inverseTable = malloc(prime * sizeof(int));

	for(int i = ident; i < prime; i++)
	{
		for(int j = ident; j < prime; j++)
		{
			if(table[i][j] == ident)
			{
				inverseTable[i] = j;
			}
		}
	}
}

int** createAdditionTable(int prime)
{
	int** table = (int**) malloc(prime * sizeof(int*));

	for(int i = 0; i < prime; i++)
	{
		table[i] = (int*) malloc(prime * sizeof(int));
		
		for(int j = 0; j < prime; j++)
		{
			int val = i + j;
			table[i][j] = val % prime;
		}
	}
	return table;
}

int** createRootTable(int** table, int prime)
{
	int** rootTable = malloc(prime * sizeof(int*));
	rootTable[0] = malloc(2 * sizeof(int));
	rootTable[0][0] = 0;
	rootTable[0][1] = 0;
	for(int i = 1; i < prime; i++)
	{
		rootTable[i] = malloc(2 * sizeof(int));
		rootTable[i][0] = -1;
		rootTable[i][1] = -1;

		int* crntRoot = rootTable[i];

		for(int j = 1; j < prime; j++)
		{
			if(table[j][j] == i)
			{
				*crntRoot = j;
				crntRoot++;
			}
		}
	}

	return rootTable;
}

int getOrderOfElliptic(int prime, int a, int b, int** mTable, int** aTable, int** rootTable)
{
	int order = 0;

	if(rootTable[b][0] != -1 && b != 0)
	{
		order += 2;
		printf("X: %i; Y: %i\n", 0, rootTable[b][0]);
		printf("X: %i; Y: %i\n", 0, rootTable[b][1]);
	}
	else if(b == 0)
	{
		order++;
		printf("X: %i; Y: %i\n", 0, 0);
	}

	for(int i = 1; i < prime; i++)
	{
		int elOne = mTable[i][i];
		elOne = mTable[elOne][i]; //cubed
		int elTwo = mTable[a][i];
		elOne = aTable[elOne][elTwo];
		elOne = aTable[elOne][b];

		if(rootTable[elOne][0] != -1 && elOne != 0)
		{
			order += 2;
			printf("X: %i; Y: %i\n", i, rootTable[elOne][0]);
			printf("X: %i; Y: %i\n", i, rootTable[elOne][1]);
		}
		else if(elOne == 0)
		{
			order++;
			printf("X: %i; Y: %i\n", i, 0);
		}

	}

	return order;
}
