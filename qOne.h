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

void printRootTable(int** table, int prime);

int getOrderOfElliptic(groupMem* mem);

groupMem* getEllipticPoints(int prime, int a, int b, int** mTable, int** aTable, int** rootTable);

