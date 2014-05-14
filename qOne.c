#include <stdio.h>
#include <stdlib.h>
#include "qOne.h"
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define ADD_IDENT 0
#define MULT_IDENT 1

//Note to self: a=8 ; b=4; p=17
int main(int argc, char*argv[])
{
	if(argc != 5)
	{
		printf("Usage: %s field A B text", argv[0]);
	} else
	{
		srand(time(NULL));

		int prime, A, B;
		prime = atoi(argv[1]);
		A = atoi(argv[2]);
		B = atoi(argv[3]);
		char* text = argv[4];

		int** mTable = createMultiplicationTable(prime);
		int* invmTable = createInverseTable(mTable, prime, MULT_IDENT);
		int** rootTable = createRootTable(mTable, prime);
		int** aTable = createAdditionTable(prime);
		int* invaTable = createInverseTable(aTable, prime, ADD_IDENT);

		printmTable(aTable, prime);
		printRootTable(invmTable, prime);
		printf("\n");

		groupMem* pointList = getEllipticPoints(prime, A, B, mTable, aTable, rootTable);

		int order = getOrderOfElliptic(pointList);

		printf("Order of elliptic: %i\n", order);

		groupMem* generator = getGenerator(pointList, A, B, mTable, aTable, invmTable, prime, order);


		printf("Generator X: %i; Y: %i\n", generator->x, generator->y);

		int personOneRand, personTwoRand;
		personTwoRand = rand() % prime;
		personOneRand = rand() % prime;

		while(personOneRand < order / 2)
		{
			personOneRand = rand() % prime;
		}

		while(personTwoRand < order / 2)
		{
			personTwoRand = rand() % prime;
		}

		groupMem* personOneDist = raiseToPower(generator, personOneRand, prime, A,
			mTable, invmTable, aTable);

		groupMem* personTwoDist = raiseToPower(generator, personTwoRand, prime, A,
			mTable, invmTable, aTable);

		groupMem* personOneSecret = raiseToPower(personTwoDist, personOneRand, prime, A,
			mTable, invmTable, aTable);

		groupMem* personTwoSecret = raiseToPower(personOneDist, personTwoRand, prime, A,
			mTable, invmTable, aTable);

		personTwoRand = rand() % prime;
		personOneRand = rand() % prime;

		while(personOneRand < 3)
		{
			personOneRand = rand() % prime;
		}

		while(personTwoRand < 3)
		{
			personTwoRand = rand() % prime;
		}

		groupMem* personOneInitDist = raiseToPower(generator, personOneRand, prime, A,
			mTable, invmTable, aTable);

		groupMem* personTwoInitDist = raiseToPower(generator, personTwoRand, prime, A,
			mTable, invmTable, aTable);

		groupMem* personOneInitSecret = raiseToPower(personTwoInitDist, personOneRand, prime, A,
			mTable, invmTable, aTable);

		groupMem* personTwoInitSecret = raiseToPower(personOneInitDist, personTwoRand, prime, A,
			mTable, invmTable, aTable);

		printf("Secret X: %i; Y: %i\n", personOneSecret->x, personOneSecret->y);
		printf("Secret X: %i; Y: %i\n", personTwoSecret->x, personTwoSecret->y);

		printf("Init X: %i; Y: %i\n", personTwoInitSecret->x, personTwoInitSecret->y);
		printf("Init X: %i; Y: %i\n", personOneInitSecret->x, personOneInitSecret->y);
		char* encrypted = encrypt(personOneSecret->x, personTwoInitSecret->x, text);

		printf("%s\n", text);
		printf("%s\n", encrypted);

		char* decrypted = decrypt(personTwoSecret->x, personTwoInitSecret->x, encrypted);
		printf("%s\n", decrypted);
	}
}

char* encrypt(int key, int vector, char* initialStr)
{
	int len = strlen(initialStr);

	char* encrypted = (char*) malloc((len * sizeof(char)) + 1);

	char temp = initialStr[0] ^ vector;

	encrypted[0] = temp ^ key;

	for(int i = 1; i < len; i++)
	{
		temp = initialStr[i] ^ encrypted[i - 1];

		encrypted[i] = temp ^ key;
	}

	encrypted[len] = '\0';
	return encrypted;
}

char* decrypt(int key, int vector, char* initialStr)
{
	int len = strlen(initialStr);

	char* encrypted = (char*) malloc((len * sizeof(char)) + 1);

	char temp = initialStr[0] ^ key;

	encrypted[0] = temp ^ vector;

	for(int i = 1; i < len; i++)
	{
		temp = initialStr[i] ^ key;

		encrypted[i] = temp ^ initialStr[i - 1];
	}

	encrypted[len] = '\0';
	return encrypted;
}

groupMem* raiseToPower(groupMem* g, int power, int prime, int a, int** mTable,
	int* invmTable, int** aTable)
{
	groupMem* crnt = doubleElliptic(g, prime, mTable, aTable, invmTable, a);
	groupMem* prev;

	for(int i = 2; i < power; i++)
	{
		prev = crnt;
		crnt = addElliptic(crnt, g, prime, mTable, aTable, invmTable);
		free(prev);
	}
	return crnt;
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

void printRootTable(int* table, int prime)
{
	printf("val\troot 1\troot 2\n");
	for(int i = 1; i < prime; i++)
	{
		printf("%i\t%i\n", i, table[i]);
	}
}

int** createMultiplicationTable(int prime)
{

	int** table = (int**) malloc(prime * sizeof(int*));

	for(int i = 0; i < prime; i++)
	{
		table[i] = (int*) malloc(prime * sizeof(int));

		for(int j = 0; j < prime; j++)
		{
			int val = i * j;
			table[i][j] = val % prime;
		}
	}
	return table;
}

int* createInverseTable(int** table, int prime, int ident)
{
	int* inverseTable = (int*) malloc(prime * sizeof(int));

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
	return inverseTable;
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

groupMem* createNode(int x, int y, groupMem* node)
{
	node->x = x;
	node->y = y;
	node->next = (groupMem*) malloc(sizeof(groupMem));
	return node->next;
}

groupMem* getEllipticPoints(int prime, int a, int b, int** mTable, int** aTable, int** rootTable)
{
	groupMem* first = (groupMem*) malloc(sizeof(groupMem));
	groupMem* crnt = first;
	groupMem* prev, tmp;

	if(rootTable[b][0] != -1 && b != 0)
	{
		printf("X: %i; Y: %i\n", 0, rootTable[b][0]);
		printf("X: %i; Y: %i\n", 0, rootTable[b][1]);
		crnt = createNode(0, rootTable[b][0], crnt);
		prev = crnt;
		crnt = createNode(0, rootTable[b][1], crnt);
	}
	else if(b == 0)
	{
		printf("X: %i; Y: %i\n", 0, 0);
		prev = crnt;
		crnt = createNode(0, 0, crnt);
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
			printf("X: %i; Y: %i\n", i, rootTable[elOne][0]);
			printf("X: %i; Y: %i\n", i, rootTable[elOne][1]);
			crnt = createNode(i, rootTable[elOne][0], crnt);
			prev = crnt;
			crnt = createNode(i, rootTable[elOne][1], crnt);
		}
		else if(elOne == 0)
		{
			printf("X: %i; Y: %i\n", i, 0);
			prev = crnt;
			crnt = createNode(i, 0, crnt);
		}

	}

	prev->next = 0;
	free(crnt);

	return first;
}

int getOrderOfElliptic(groupMem* mem)
{
	int order = 0;
	groupMem* crnt = mem;

	do
	{
		order++;
		crnt = crnt->next;
	}
	while(crnt != 0);
	//for the point at infinity
	order += 1;
	return order;
}

groupMem* getGenerator(groupMem* list, int a, int b, int** mTable, int** aTable,
	int* invmTable, int prime, int order)
{
	groupMem* crnt = list;

	do
	{
		if(crnt->y != 0)
		{
			int elOrder = 2;
			int invX = crnt->x;
			int invY = prime - crnt->y;
			groupMem* p = doubleElliptic(crnt, prime, mTable, aTable, invmTable, a);
			groupMem* prev;
			printf("mem X: %i; Y:%i; order reached: %i\n", p->x, p->y, elOrder);
			while(p->x != invX)
			{
				prev = p;
				p = addElliptic(crnt, p, prime, mTable, aTable, invmTable);
				free(prev);
				elOrder++;
				printf("mem X: %i; Y:%i; order reached: %i\n", p->x, p->y, elOrder);
			}
			elOrder++;
			if(elOrder == order)
			{
				return crnt;
			}

		}
		crnt = crnt->next;
		printf("Changing point\n");
	}
	while(crnt != 0);
	assert(1==3);
}

groupMem* addElliptic(groupMem* one, groupMem* two, int prime, int** mTable,
	int** aTable, int* invmTable)
{
	int yDif = (one->y - two->y  + prime) % prime;
	int xDif = (one->x - two->x  + prime) % prime;
	printf("a\n");
	int slope = mTable[yDif][invmTable[xDif]];
	printf("a\n");
	int slopeSq = mTable[slope][slope];
	printf("%i\n", slopeSq);
	int newX = slopeSq - (one->x + two->x);
	newX = newX + prime;
	newX = newX % prime;
	printf("a\n");
	int newY = mTable[slope][((newX - one->x) + prime)  % prime];
	printf("%i AND %i\n", newY, newX);
	newY = aTable[newY][one->y];
	printf("a\n");
	newY = prime - newY;
	groupMem* newEl = (groupMem*) malloc(sizeof(groupMem));
	newEl->x = newX;
	newEl->y = newY;
	printf("end\n");
	return newEl;

}

groupMem* doubleElliptic(groupMem* one, int prime, int** mTable,
	int** aTable, int* invmTable, int a)
{
	//general form slope: slope= (3x^2 + a)/2y
	int firstTerm = mTable[3][mTable[one->x][one->x]];
	//printf("a\n");
	int denominator = mTable[2][one->y];
	//printf("b\n");
	denominator = invmTable[denominator];
	//printf("b\n");
	int slope = mTable[aTable[firstTerm][a]][denominator];
	//printf("c\n");
	int slopeSq = mTable[slope][slope];
	//printf("d\n");
	int newX = aTable[slopeSq][prime - aTable[one->x][one->x]];
	int newY = prime - aTable[mTable[slope][aTable[newX][prime - one->x]]][one->y];
	//printf("e\n");
	groupMem* newEl = (groupMem*) malloc(sizeof(groupMem));
	newEl->x = newX;
	newEl->y = newY;
	return newEl;
}
