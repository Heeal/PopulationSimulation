#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "inc/Distribution.h"
#include "inc/Utils.h"
#include "inc/struct.h"


#define MIN_COUNT_FOR_FIRSTNAME 500

void CreateFirstNameDatabase();
FirstNameDistribution* CreateFirstNameDistribution();
void CalculateFirstnameProportions();
void PrintDisitribution(FirstNameDistribution* FirstNameLaw);
char* GenerateRandomFirstname(unsigned short Gender);

FirstNameDistribution* Law;
unsigned long long int TotalDistinct[2] = {0, 0};
unsigned long long int TotalCount[2] = {0, 0};

FirstNameDistribution* AllocLaw(){
    FirstNameDistribution* newlaw = (FirstNameDistribution*) malloc(sizeof(FirstNameDistribution));
    return newlaw;
}

void DataManagementInit(){
    /* Create FirstName Random Metrics*/
    srand(time(NULL));
    CreateFirstNameDatabase();
    Law = CreateFirstNameDistribution();
    DEBUG_INFO("Finished\n");
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
    FirstNameDistribution* FirstNameLaw = AllocLaw();

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

unsigned char* GenerateRandChild(Human* h){
    int random_child;
    unsigned char* NumberOfChildAt = malloc(35);
    for(int i = 0; i<35;i++){
        NumberOfChildAt[i] = 0;
    }
    // If Age < 15, return 0
    if(h->Age < 15) return 0;

    // If Age > 15, Calculate probabilty of child at every passed age
    for(unsigned char i=0; i<(h->Age-15); i++){
        if(i>=35) return NumberOfChildAt;

        // Generate Random Float with 1 decimal in [0;100] (equivalent generate int in [1;1000])
        random_child = rand()%1000;
        if (random_child <= ProbabilityOfChilds[i]*10){
            NumberOfChildAt[i]++;
        }
    }

    return NumberOfChildAt;
}

unsigned short GenerateRandomGender(){
    return rand()%2;
}

unsigned short GenerateRandomAge(int MinAge, int MaxAge){
    return ((rand()%(MaxAge - MinAge + 1)) + MinAge);
}

char* GenerateRandomFirstname(unsigned short Gender){
    int random_int = rand()%Law->Total[Gender];
    char* random_name = malloc(FIRSTNAME_MAX_SIZE);
    memcpy(random_name, Law->FirstNames[Gender][Law->Distribution[Gender][random_int]], FIRSTNAME_MAX_SIZE);
    return random_name;
}


void PrintDisitribution(FirstNameDistribution* FirstNameLaw){
    for(unsigned long long int i = 0; i < FirstNameLaw->Total[GENDER_FEMALE] ; i++){
        printf("Distribution[%lld] = %s (%lld)\n", i, FirstNameLaw->FirstNames[GENDER_FEMALE][FirstNameLaw->Distribution[GENDER_FEMALE][i]], FirstNameLaw->Distribution[GENDER_FEMALE][i]);
    }
}

void PrintHuman(Human* h){
    printf("Firstname: %s\n", h->FirstName);
    printf("Lastname : TBD\n");
    printf("Gender   : ");
    if(h->Gender == GENDER_MALE){
        printf("MALE\n");
    }else if(h->Gender == GENDER_FEMALE){
        printf("FEMALE\n");
    }
    printf("Age      : %d\n", h->Age);

    if(h->Partner != NULL){
        printf("Couple   : %s \n", h->Partner->FirstName);
    }

    if(h->Father != NULL){
        printf("Father   : %s \n", h->Father->FirstName);
    }
    if(h->Mother != NULL){
        printf("Mother   : %s \n", h->Mother->FirstName);
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