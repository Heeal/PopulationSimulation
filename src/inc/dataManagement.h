Human* AllocateHuman();
void DataManagementInit();
unsigned short GenerateRandomGender();
char* GenerateRandomFirstname(unsigned short Gender);
unsigned short GenerateRandomAge(int MinAge, int MaxAge);
unsigned char GenerateRandIsInCouple(Human* h);
unsigned char* GenerateRandChild(Human* h);
void PrintHuman(Human* h);
void PrintCondensateHuman(Human* h);
