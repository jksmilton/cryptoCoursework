typedef struct groupMem
{
	int x;
	int y;
	struct groupMem* next;
} groupMem;

int** createMultiplicationTable(int prime);

int* createInverseTable(int** table, int prime, int ident);

int** createAdditionTable(int prime);

void printmTable(int** table, int prime);

groupMem* createNode(int x, int y, groupMem* node);

int** createRootTable(int** table, int prime);

void printRootTable(int* table, int prime);

int getOrderOfElliptic(groupMem* mem);

groupMem* getEllipticPoints(int prime, int a, int b, int** mTable, int** aTable, int** rootTable);

groupMem* getGenerator(groupMem* list, int a, int b, int** mTable, int** aTable,
	int* invmTable, int prime, int order);

groupMem* addElliptic(groupMem* one, groupMem* two, int prime, int** mTable,
	int** aTable, int* invmTable);

groupMem* doubleElliptic(groupMem* one, int prime, int** mTable,
	int** aTable, int* invmTable, int a);

groupMem* raiseToPower(groupMem* g, int power, int prime, int a, int** mTable,
	int* invmTable, int** aTable);

char* encrypt(int key, int vector, char* initialStr);

char* decrypt(int key, int vector, char* initialStr, int len);