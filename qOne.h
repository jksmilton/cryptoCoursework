int** createMultiplicationTable(int prime);

int* createInverseTable(int** table, int prime, int ident);

int** createAdditionTable(int prime);

void printmTable(int** table, int prime);

int** createRootTable(int** table, int prime);

void printRootTable(int** table, int prime);

int getOrderOfElliptic(int prime, int a, int b, int** mTable, int** aTable, int** rootTable);