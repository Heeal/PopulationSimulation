#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "inc/Distribution.h"
#include "inc/Utils.h"
#include "inc/struct.h"


#define MIN_COUNT_FOR_FIRSTNAME 500

void CreateFirstNameDatabase();
void CreateLastNameDatabase();
FirstNameDistribution* CreateFirstNameDistribution();
void CalculateFirstnameProportions();
void PrintDisitribution(FirstNameDistribution* FirstNameLaw);
char* GenerateRandomFirstname(unsigned short Gender);

FirstNameDistribution* FirstNameLaw;
LastNameDistribution* LastNameLaw;
unsigned long long int TotalDistinct[2] = {0, 0};
unsigned long long int TotalCount[2] = {0, 0};

FirstNameDistribution* AllocFirstNameLaw(){
    FirstNameDistribution* newlaw = (FirstNameDistribution*) malloc(sizeof(FirstNameDistribution));
    return newlaw;
}

LastNameDistribution* AllocLastNameLaw(){
    LastNameDistribution* newlaw = (LastNameDistribution*) malloc(sizeof(LastNameDistribution));
    newlaw->Size = 0;
    return newlaw;
}

void DataManagementInit(){
    /* Create FirstName Random Metrics*/
    srand(time(NULL));
    CreateFirstNameDatabase();
    CreateLastNameDatabase();
    FirstNameLaw = CreateFirstNameDistribution();
    DEBUG_INFO("Finished\n");
}

#define BUF_SIZE 65536

int count_lines(FILE* file)
{
    char buf[BUF_SIZE];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, BUF_SIZE, file);
        if (ferror(file))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(file))
            break;
    }
    fclose(file);
    return counter;
}

void CreateLastNameDatabase(){
    LastNameLaw = AllocLastNameLaw();
    DEBUG_INFO("Reading Lastname Database\n");
    FILE* tmp = fopen("./data/lastname.csv", "r");
    if(tmp==NULL){
        DEBUG_ERROR("Error while opening database file : lastname.csv\n");
        exit(-1);
    }

    int nbLastNames = count_lines(tmp);
    LastNameLaw->LastNames = malloc(nbLastNames * sizeof(char*));

    DEBUG_INFO("%d Last names\n", nbLastNames);
    FILE* DataFileLastName = fopen("./data/lastname.csv", "r");
    if(DataFileLastName==NULL){
        DEBUG_ERROR("Error while opening database file : lastname.csv\n");
        exit(-1);
    }
    
    unsigned int i = 0;
    char* LastName = malloc(50);
    char line[250];

    // First line to trash
    fgets(line, sizeof(line), DataFileLastName);

    // Read through all lines
    while (fgets(line, sizeof(line), DataFileLastName) != NULL){
        LastNameLaw->LastNames[i] = (char*) malloc(50);
        sscanf(line, "%s", LastName);
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        memcpy(LastNameLaw->LastNames[i], line, 50);
        LastNameLaw->Size++;
        i++;
    }
    DEBUG_INFO("Finished reading database file : lastname.csv %lld\n", LastNameLaw->Size);
    fclose(DataFileLastName);
}

void CreateFirstNameDatabase(){
    DEBUG_INFO("Reading Firstname Database\n");
    FILE* DataFileFirstName = fopen("./data/firstname.csv", "r");
    FILE* OptimisedDataFileFirstName = fopen("./data/OptimisedDataFileFirstName.csv", "w");
    if(DataFileFirstName==NULL){
        DEBUG_ERROR("Error while opening database file : firstname.csv\n");
        exit(-1);
    }
    
    unsigned int i = 0;
    unsigned short PreviousGender;
    unsigned short Gender;
    char PreviousFirstName[FIRSTNAME_MAX_SIZE] = "\0";
    char FirstName[FIRSTNAME_MAX_SIZE] = "\0";
    char Year[5];
    unsigned int Count;
    unsigned int TotalCountForFirstname = 0;
    char line[250];

    // First line to trash
    fgets(line, sizeof(line), DataFileFirstName);

    // Read through all lines
    while (fgets(line, sizeof(line), DataFileFirstName) != NULL){
        sscanf(line, "%hd %s %s %d ", &Gender, FirstName, Year, &Count);
        // If first element
        if(PreviousFirstName[0] == '\0'){
            PreviousGender = Gender;
            memcpy(PreviousFirstName, FirstName, FIRSTNAME_MAX_SIZE);
        }

        // If element is same as previous
        if(strcmp(FirstName, PreviousFirstName)==0){
            TotalCountForFirstname+=Count;
        
        }else{
            // Else, store previous element total count
            if(TotalCountForFirstname > MIN_COUNT_FOR_FIRSTNAME){
                fprintf(OptimisedDataFileFirstName, "%hd %s %d\n", PreviousGender, PreviousFirstName, TotalCountForFirstname);

                if(Gender==1){
                    TotalDistinct[GENDER_MALE]+=1;
                    TotalCount[GENDER_MALE]+=TotalCountForFirstname;
                }else{
                    TotalDistinct[GENDER_FEMALE]+=1;
                    TotalCount[GENDER_FEMALE]+=TotalCountForFirstname;
                }
            }
            PreviousGender = Gender;
            memcpy(PreviousFirstName, FirstName, FIRSTNAME_MAX_SIZE);
            Count = 0;
            TotalCountForFirstname = 0;
        }
        i++;
    }
    DEBUG_INFO("Finished reading database file : firstname.csv\n");
    DEBUG_INFO("Created optimised database file for next time : OptimisedDataFileFirstName.csv\n");
    fclose(DataFileFirstName);
    fclose(OptimisedDataFileFirstName);
}


FirstNameDistribution* CreateFirstNameDistribution(){
    FirstNameDistribution* FirstNameLaw = AllocFirstNameLaw();

    FirstNameLaw->Size[GENDER_MALE] = TotalDistinct[GENDER_MALE];
    FirstNameLaw->Size[GENDER_FEMALE] = TotalDistinct[GENDER_FEMALE];
    FirstNameLaw->Size[GENDER_ALL] = TotalDistinct[GENDER_ALL];

    FirstNameLaw->Total[GENDER_MALE] = TotalCount[GENDER_MALE];
    FirstNameLaw->Total[GENDER_FEMALE] = TotalCount[GENDER_FEMALE];
    FirstNameLaw->Total[GENDER_ALL] = TotalCount[GENDER_ALL];
    FirstNameLaw->FirstNames[GENDER_MALE] = (char**) malloc(FirstNameLaw->Size[GENDER_MALE] * sizeof(char*));
    FirstNameLaw->FirstNames[GENDER_FEMALE] = (char**) malloc(FirstNameLaw->Size[GENDER_FEMALE] * sizeof(char*));

    //For each gender
    for(short j = 0; j < 2 ; j++){
        // For each possible name
        for(unsigned long long int i = 0; i < FirstNameLaw->Size[j] ; i++){
            // Allocate FIRSTNAME_MAX_SIZE 
            FirstNameLaw->FirstNames[j][i] = (char*) malloc(FIRSTNAME_MAX_SIZE);
        }

        // Allocate Distribution Array
        FirstNameLaw->Distribution[j] = (unsigned long long int*) malloc(TotalCount[j] * sizeof(unsigned long long int));
    }

    CalculateFirstnameProportions(FirstNameLaw);
    return FirstNameLaw;
}

void CalculateFirstnameProportions(FirstNameDistribution* FirstNameLaw){
    FILE* OptimisedDataFileFirstName = fopen("./data/OptimisedDataFileFirstName.csv", "r");
    if(OptimisedDataFileFirstName==NULL){
        DEBUG_ERROR("Error while opening database file : firstname.csv\n");
        exit(-1);
    }
    unsigned long long int n = 0;
    unsigned short Gender;
    char FirstName[FIRSTNAME_MAX_SIZE] = "\0";
    unsigned int Count;
    char line[250];

    for(unsigned short j = 0; j < 2; j++){
        for(unsigned long long int i = 0 ; i < FirstNameLaw->Size[j] ; i++){
            fgets(line, sizeof(line), OptimisedDataFileFirstName);
            sscanf(line, "%hd %s %d ", &Gender, FirstName, &Count);
            memcpy(FirstNameLaw->FirstNames[j][i], FirstName, FIRSTNAME_MAX_SIZE);
            // printf("Proportion of %s in population : %.9f (count=%d, total=%lld)\n", FirstNameLaw->FirstNames[j][i],(((double)Count)/TotalCount[j]), Count, TotalCount[j]);
            
            // For each count
            // printf("Add %d entries for %s to distribution\n", Count, FirstNameLaw->FirstNames[j][i]);
            for(int k = 0; k < Count ; k++){
                // Add index to Distribution law per gender
                FirstNameLaw->Distribution[j][n+k] = i;
            }
            n+=Count;
        }
        n=0;
    }
}

unsigned char GenerateRandIsInCouple(Human* h){
    unsigned char random_couple = rand()%100;
    unsigned char IsInCouple = 0;
    // If Age < 18 => Not in couple
    if(h->Age < 18){
        IsInCouple = 0;
    }
    else if(h->Age >= 18 && h->Age <= 24){
        IsInCouple = (random_couple >= PROBABILITY_OF_COUPLE_18_24) ? 0 : 1;
    }
    else if(h->Age >= 25 && h->Age <= 34){
        IsInCouple = (random_couple >= PROBABILITY_OF_COUPLE_25_34) ? 0 : 1;
    }
    else if(h->Age >= 35 && h->Age <= 44){
        IsInCouple = (random_couple >= PROBABILITY_OF_COUPLE_35_44) ? 0 : 1;
    }
    else if(h->Age >= 45 && h->Age <= 54){
        IsInCouple = (random_couple >= PROBABILITY_OF_COUPLE_45_54) ? 0 : 1;
    }
    else if(h->Age >= 55 && h->Age <= 64){
        IsInCouple = (random_couple >= PROBABILITY_OF_COUPLE_55_64) ? 0 : 1;
    }
    else if(h->Age >= 65 && h->Age <= 74){
        IsInCouple = (random_couple >= PROBABILITY_OF_COUPLE_65_74) ? 0 : 1;
    }
    else if(h->Age >= 75){
        IsInCouple = (random_couple >= PROBABILITY_OF_COUPLE_75_OLDER) ? 0 : 1;
    }
    return IsInCouple;
}

int CalculateImmigrationSize(Population* pop){
    float random = (float)((rand()%(9)) + 1)/10000;
    return (pop->Size*(POUCENTAGE_IMMIGRATION+random));
}

int GenerateRandChild(Human* h){
    int random_child;
    int NumberOfChild = 0;

    // If Age < 15 or > 50, returns 0
    if(h->Age < 15) return 0;
    if(h->Age > 50) return 0;

    // Generate Random Float with 1 decimal in [0;100] (equivalent generate int in [1;1000])
    random_child = rand()%1000;
    if (random_child <= ProbabilityOfChilds[h->Age-15]*10){
        NumberOfChild++;
        // Twins ???
        random_child = rand()%1000;
        if(random_child<=15){
            NumberOfChild++;
        }
    }
    return NumberOfChild;
}

unsigned short GenerateRandomGender(){
    return rand()%2;
}

unsigned short GenerateRandomAge(int MinAge, int MaxAge){
    return ((rand()%(MaxAge - MinAge + 1)) + MinAge);
}

unsigned char GenerateRandomDeath(Human* h){
    int random_death = rand()%10000;

    if (h->Age < 104){
        if (random_death <= ProbabilityOfDeath[h->Age][h->Gender]){
            return 1; 
        }
    }else{
        if (random_death <= ProbabilityOfDeath[104][h->Gender]){
            return 1; 
        }
    }
    return 0;
}

short GenerateRandomDNA(){
    short DNA = 0;
    char hair_color = rand()%4;
    char eyes_color = rand()%4;
    char morphotype = rand()%3;
    char blood_type = rand()%4;

    DNA |= hair_color << DNA_HAIR_COLOR;
    DNA |= eyes_color << DNA_EYES_COLOR;
    DNA |= morphotype << DNA_MORPHOTYPE;
    DNA |= blood_type << DNA_BLOODTYPE;

    return DNA;
}

char* GenerateRandomFirstname(unsigned short Gender){
    int random_int = rand()%FirstNameLaw->Total[Gender];
    char* random_name = malloc(FIRSTNAME_MAX_SIZE);
    memcpy(random_name, FirstNameLaw->FirstNames[Gender][FirstNameLaw->Distribution[Gender][random_int]], FIRSTNAME_MAX_SIZE);
    return random_name;
}

char* GenerateRandomLastName(){
    int random_int = rand()%LastNameLaw->Size;
    char* random_name = malloc(50);
    memcpy(random_name, LastNameLaw->LastNames[random_int], 50);
    return random_name;
}


void PrintDisitribution(FirstNameDistribution* FirstNameLaw){
    for(unsigned long long int i = 0; i < FirstNameLaw->Total[GENDER_FEMALE] ; i++){
        printf("Distribution[%lld] = %s (%lld)\n", i, FirstNameLaw->FirstNames[GENDER_FEMALE][FirstNameLaw->Distribution[GENDER_FEMALE][i]], FirstNameLaw->Distribution[GENDER_FEMALE][i]);
    }
}

void PrintHuman(Human* h){
    printf("Firstname: %s\n", h->FirstName);
    printf("Lastname : %s\n", h->LastName);
    printf("Gender   : ");
    if(h->Gender == GENDER_MALE){
        printf("MALE\n");
    }else if(h->Gender == GENDER_FEMALE){
        printf("FEMALE\n");
    }
    printf("Age      : %d\n", h->Age);
    printf("DNA      : %d\n", h->DNA);

    if(h->Partner != NULL){
        printf("Couple   : %s %s\n", h->Partner->FirstName, h->Partner->LastName);
    }

    if(h->Parents[GENDER_MALE] != NULL){
        printf("Father   : %s %s\n", h->Parents[GENDER_MALE]->FirstName, h->Parents[GENDER_MALE]->LastName);
    }
    if(h->Parents[GENDER_FEMALE] != NULL){
        printf("Mother   : %s %s\n", h->Parents[GENDER_FEMALE]->FirstName, h->Parents[GENDER_FEMALE]->LastName);
    }

    printf("Nb of childs : %d\n", h->Childs->Size);
    HumanListElement* child = h->Childs->First;
    for(int i=0;i<h->Childs->Size;i++){
        printf("Child %d :\n",i);
        PrintHuman(child->Info);
        child = child->next;
    }

    printf("\n\n");
}

void PrintCondensateHuman(Human* h){
    printf("[");
    if(h->Gender == GENDER_MALE){
        printf("M");
    }else if(h->Gender == GENDER_FEMALE){
        printf("F");
    }
    printf("%d] %s", h->Age, h->FirstName);
}

void Statistics(Population* pop, Stats* s){
    HumanListElement* current = pop->First;
    
    unsigned int TotalMale = 0;
    unsigned int TotalFemale = 0;
    Human* MostAged = current->Info;
    Human* Mostchildrens = current->Info;
    unsigned long long int SumAge = 0;
    unsigned long long int SumChildrens = 0;
    long double MeanAge = 0;
    long double MeanChildrens = 0;

    while(current!=NULL){
        SumAge += current->Info->Age;
        if(current->Info->Gender == GENDER_MALE){
            TotalMale++;
        }else {
            SumChildrens += current->Info->Childs->Size;
            TotalFemale++;
        }
        if(current->Info->Age > MostAged->Age){
            MostAged = current->Info;
        }
        if(current->Info->Childs->Size > Mostchildrens->Childs->Size){
            Mostchildrens = current->Info;
        }
        current = current->next;
    }
    MeanAge = (long double) SumAge/pop->Size;
    MeanChildrens = (long double) SumChildrens/TotalFemale;

    int NewPopulationSize = pop->Size;
    int Delta = (NewPopulationSize-(s->InitialPopulationSize));
    double Growth = (double)(Delta*100)/(s->InitialPopulationSize);


    
    s->TotalMale = TotalMale;
    s->TotalFemale = TotalFemale;
    s->Growth = Growth;
    s->MeanAge = MeanAge;
    s->MostAged = MostAged;
    s->MostChildrens = Mostchildrens;
    s->MeanChildrens = MeanChildrens;
}