#include <stdio.h>
#include <stdlib.h>

#include "inc/struct.h"
#include "inc/dataManagement.h"


#define MIN_AGE 0
#define MAX_AGE 120

#define MIN_AGE_OLDER 45
#define MAX_AGE_OLDER 100


Human* GenerateRandomHumanFirstGen();
Human* GenerateRandomHuman();
void PrintPopulation(Population* pop);
Population* AllocPopulation();
void GenerateCouple(Population* pop);
void GenerateChilds(Population* pop);


Population* FirstGeneration;
Population* TotalPopulation;

Population* AllocPopulation(){
    // Init population
    Population* pop = malloc(sizeof(Population));
    pop->Size = 0;
    return pop;
}

Human* AllocateHuman(){
    Human* human = (Human*) malloc(sizeof(Human));
    human->Childs = AllocPopulation();
    return human;
}

HumanListElement* AllocHumanListElement(Human* h){
    // Init pop
    HumanListElement* HumanElement = malloc(sizeof(HumanListElement));
    HumanElement->Info = h;
    HumanElement->next = NULL;
    return HumanElement;
}

void AddHumanToPopulation(Population* pop, Human* h){
    HumanListElement* HumanElement = AllocHumanListElement(h);
    if(pop->Size==0){
        pop->First = HumanElement;
    }else{
        HumanListElement* current = pop->First;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = HumanElement;
    }
    pop->Size++;
}

Population* GenerateOlderPopulation(unsigned int NbOfPeople){
    Population* SubPopulation_FirstGeneration = AllocPopulation();
    
    for(int i = 0; i< NbOfPeople ; i++){
        Human* h = GenerateRandomHumanFirstGen();
        AddHumanToPopulation(SubPopulation_FirstGeneration, h);
    }
    return SubPopulation_FirstGeneration;
}

Population* GeneratePopulation(unsigned int NbOfPeople){
    TotalPopulation = AllocPopulation();
    
    // Generate First Generation
    FirstGeneration = GenerateOlderPopulation(NbOfPeople);
    GenerateCouple(FirstGeneration);
    GenerateChilds(FirstGeneration);
    
    return TotalPopulation;
}

void PrintPopulation(Population* pop){
    HumanListElement* current = pop->First;
    printf("Population size : %d\n", pop->Size);
    for(int i = 0; i< pop->Size ; i++){
        PrintCondensateHuman(current->Info);
        if(current->Info->Partner != NULL){
            printf(" + ");
            PrintCondensateHuman(current->Info->Partner);
        }
        HumanListElement* current_child = current->Info->Childs->First;
        for(int i_child=0 ; i_child<current->Info->Childs->Size;i_child++){
            printf("\n\t|--- ");
            PrintCondensateHuman(current_child->Info);
            current_child = current_child->next;
        }
        current = current->next;
        printf("\n\n");
    }
}

Human* GenerateRandomHumanByAge(unsigned char MinAge, unsigned char MaxAge){
    Human* h = AllocateHuman();
    h->Gender = GenerateRandomGender();
    h->FirstName = GenerateRandomFirstname(h->Gender);
    h->LastName = "TBD";
    // Lastname to be set when generating family tree
    h->Age = GenerateRandomAge(MinAge, MaxAge);
    AddHumanToPopulation(TotalPopulation, h);
    return h;
}

Human* GenerateRandomHumanFirstGen(){
    return GenerateRandomHumanByAge(MIN_AGE_OLDER, MAX_AGE_OLDER);
}


Human* GenerateRandomHumanByGender(unsigned short Gender, unsigned char Age){
    Human* h = AllocateHuman();
    h->Gender = Gender;
    h->FirstName = GenerateRandomFirstname(h->Gender);
    h->LastName = "TBD";
    // Lastname to be set when generating family tree
    h->Age = GenerateRandomAge(Age-10, Age+10);
    AddHumanToPopulation(TotalPopulation, h);
    return h;
}


Human* GenerateRandomHuman(){
    Human* h = AllocateHuman();
    h->Gender = GenerateRandomGender();
    h->FirstName = GenerateRandomFirstname(h->Gender);
    h->LastName = "TBD";
    // Lastname to be set when generating family tree
    h->Age = rand()%MAX_AGE;
    AddHumanToPopulation(TotalPopulation, h);
    return h;
}

void GenerateCouple(Population* pop){
    printf("Generating couples...\n");
    HumanListElement* current = pop->First;
    for(int i = 0; i< pop->Size ; i++){
        if(GenerateRandIsInCouple(current->Info) == 1){
            // Create Human of opposite gender
            Human* Couple = GenerateRandomHumanByGender(current->Info->Gender^1, current->Info->Age);
            AddHumanToPopulation(pop, Couple);
            Couple->Partner = current->Info;
            current->Info->Partner = Couple;
        }
        current = current->next;
    }
    printf("Done\n");
}


void GenerateChilds(Population* pop){
    printf("Generating N+1 generation...\n");
    HumanListElement* current = pop->First;

    for(int i = 0; i< pop->Size ; i++){
        //For every woman, we calculate how many childs she probably had at its age
        if(current->Info->Gender == GENDER_FEMALE){
            unsigned char* HadAChildAt = GenerateRandChild(current->Info);
            int totalChilds = 0;

            //For every year of fecondity (15-50)
            for(int j = 0; j < 35; j++){
                int nbChildAt = HadAChildAt[j];

                // For every child she had that year
                for(int k=0;k<nbChildAt;k++){
                    // Create a Child
                    totalChilds++;
                    Human* child = AllocateHuman();
                    child = GenerateRandomHumanByAge((current->Info->Age-(15+j)), (current->Info->Age-(15+j)));
                    child->Mother = current->Info;
                    child->Father = current->Info->Partner;
            
                    // Add Child to Population of childs
                    AddHumanToPopulation(current->Info->Childs, child);
                    if(current->Info->Partner!=NULL){
                        AddHumanToPopulation(current->Info->Partner->Childs, child);;
                    }
                }
            }
        }
        current = current->next;
    }
    printf("Done\n");
}