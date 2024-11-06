Human* AllocateHuman();
void DataManagementInit();
unsigned short GenerateRandomGender();
char* GenerateRandomFirstname(unsigned short Gender);
char* GenerateRandomLastName();
unsigned short GenerateRandomAge(int MinAge, int MaxAge);
unsigned char GenerateRandIsInCouple(Human* h);
short GenerateRandomDNA();
int GenerateRandChild(Human* h);
void PrintHuman(Human* h);
void PrintCondensateHuman(Human* h);
unsigned char GenerateRandomDeath(Human* h);
int CalculateImmigrationSize(Population* pop);
void Statistics(Population* pop, Stats* s);