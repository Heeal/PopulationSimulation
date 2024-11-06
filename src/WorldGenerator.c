#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/struct.h"
#include "inc/Utils.h"
#include "inc/dataManagement.h"


#define MIN_AGE 0
#define MAX_AGE 70

#define MIN_AGE_OLDER 45
#define MAX_AGE_OLDER 100


Human* GenerateRandomHumanFirstGen();
Human* GenerateRandomHuman();
void PrintPopulation(Population* pop);
Population* AllocPopulation();
int GenerateCouple(Population* pop);
int GenerateChilds(Population* pop);
void PrintStats(Stats* s, int year);
void CheckAnomalies(Population* pop);

Population* TotalPopulation;

Population* AllocPopulation(){
    // Init population
    Population* pop = malloc(sizeof(Population));
    pop->First = NULL;
    pop->Last = NULL;
    pop->Size = 0;
    return pop;
}

Human* AllocateHuman(){
    Human* human = (Human*) malloc(sizeof(Human));
    human->Childs = AllocPopulation();
    return human;
}

void DeallocatePopulation(Population* pop){
    HumanListElement* current = pop->First;
    HumanListElement* tmp;
    for(int i = 0; i< pop->Size ; i++){
        tmp = current;
        current = current->next;
        free(tmp);
    }
}


void DeallocateHuman(Human* h){
    DeallocatePopulation(h->Childs);
    free(h->FirstName);
    free(h);
}

HumanListElement* AllocHumanListElement(Human* h){
    // Init pop
    HumanListElement* HumanElement = malloc(sizeof(HumanListElement));
    HumanElement->Info = h;
    HumanElement->next = NULL;
    HumanElement->previous = NULL;
    return HumanElement;
}

void AddHumanToPopulation(Population* pop, Human* h){
    HumanListElement* HumanElement = AllocHumanListElement(h);

    if(pop->Size==0){
        pop->First = HumanElement;
    }else{
        HumanElement->previous = pop->Last;
        pop->Last->next = HumanElement;
    }
    pop->Last = HumanElement;
    pop->Size++;
}

void RemoveHumanFromPopulation(Population* pop, HumanListElement* h){
    HumanListElement* p = h->previous;
    HumanListElement* n = h->next;

    // h is last element
    if(n==NULL && p!=NULL){
        p->next =  NULL;
        pop->Last = p;
    }

    // h is first element
    else if(p==NULL && n!=NULL){
        n->previous = NULL;
        pop->First = n;
    }

    // h is the only element
    else if(p==NULL && n==NULL){
        pop->First = NULL;
        pop->Last = NULL;
    }

    else{
        p->next = n;
        n->previous = p;
    }

    pop->Size--;
}

Population* GeneratePopulation(unsigned int NbOfPeople){
    TotalPopulation = AllocPopulation();
    for(int i = 0; i< NbOfPeople ; i++){
        GenerateRandomHumanFirstGen();
    }
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

Human* GenerateRandomChild(Human* Father){
    Human* h = AllocateHuman();
    h->Gender = GenerateRandomGender();
    h->FirstName = GenerateRandomFirstname(h->Gender);
    h->LastName = malloc(50);
    strcpy(h->LastName, Father->LastName);
    h->Age = 0;
    AddHumanToPopulation(TotalPopulation, h);
    return h;
}


Human* GenerateRandomHumanByAge(unsigned char MinAge, unsigned char MaxAge){
    Human* h = AllocateHuman();
    h->Gender = GenerateRandomGender();
    h->FirstName = GenerateRandomFirstname(h->Gender);
    h->LastName = GenerateRandomLastName();
    // Lastname to be set when generating family tree
    h->Age = GenerateRandomAge(MinAge, MaxAge);
    AddHumanToPopulation(TotalPopulation, h);
    return h;
}

Human* GenerateRandomHumanFirstGen(){
    return GenerateRandomHumanByAge(MIN_AGE, MAX_AGE);
}

Human* GenerateRandomHuman(){
    Human* h = AllocateHuman();
    h->Gender = GenerateRandomGender();
    h->FirstName = GenerateRandomFirstname(h->Gender);
    h->LastName = GenerateRandomLastName();
    // Lastname to be set when generating family tree
    h->Age = rand()%MAX_AGE;
    AddHumanToPopulation(TotalPopulation, h);
    return h;
}


int abs(int v) 
{
  return v * ((v>0) - (v<0));
}

Human* FindMatchingCouple(Population* pop, Human* h){
    if(pop->Size<2) return 0;
    HumanListElement* current = pop->First;
    for(int i = 0; i< pop->Size ; i++){
        // If current has no Partner
        if(current->Info->Partner == NULL){
            // If opposite gender
             if((current->Info->Gender ^ h->Gender) == 1){
                // If not from the same family
                if(strcmp(current->Info->LastName, h->LastName) != 0){
                    // If less than 10 years of difference
                    if(abs(current->Info->Age - h->Age) < 15 && current->Info->Age >= 15){
                        return current->Info;
                    }
                }
            }
        }  
        current = current->next;
    }

    // Not any single matching soul :'(
    return NULL;
}

int GenerateCouple(Population* pop){
    if(pop->Size<2) return 0;
    HumanListElement* current = pop->First;
    int TotalNewCouples = 0;
    for(int i = 0; i< pop->Size ; i++){
        if(current->Info->Partner == NULL){
            if(GenerateRandIsInCouple(current->Info) == 1){
                // Create Human of opposite gender
                Human* Couple = FindMatchingCouple(pop, current->Info);
                if(Couple != NULL){
                    Couple->Partner = current->Info;
                    current->Info->Partner = Couple;
                    TotalNewCouples++;
                }
            }
        }
        current = current->next;
    }
    return TotalNewCouples;
    // printf("Total new couple this year : %d\n", TotalNewCouples);
}

int GenerateChilds(Population* pop){
    if(pop->Size<2) return 0;
    HumanListElement* current = pop->First;
    int TotalNewBorn = 0;
    while(current != NULL){
        //For every woman, we calculate how many childs she probably have at their age
        if(current->Info->Gender == GENDER_FEMALE && current->Info->Partner != NULL){
            if(current->Info->Partner->Age >= 15){
                // printf("New iteration\n");
                int NbOfChilds = GenerateRandChild(current->Info);
                // For every child she had that year
                for(int k=0;k<NbOfChilds;k++){
                    // Create a Child
                    Human* child = AllocateHuman();
                    child = GenerateRandomChild(current->Info->Partner);

                    // printf("New Child from %s %s and %s %s => %s %s\n", current->Info->FirstName, current->Info->LastName, current->Info->Partner->FirstName, current->Info->Partner->LastName, child->FirstName, child->LastName);
                    child->Parents[GENDER_FEMALE] = current->Info;
                    child->Parents[GENDER_MALE] = current->Info->Partner;

                    // Add Child to Population of childs
                    //printf("New children %s <3 %s => %s\n", current->Info->FirstName, current->Info->Partner->FirstName, child->FirstName);
                    AddHumanToPopulation(current->Info->Childs, child);
                    AddHumanToPopulation(current->Info->Partner->Childs, child);
                    TotalNewBorn++;
                }
            }
        }
        current = current->next;
    }
    return TotalNewBorn;
}

int GenerateDeath(Population* pop){
    HumanListElement* current = pop->First;
    if(pop->Size<1) return 0;
    
    int TotalDeaths = 0;
    while(current!=NULL){
        char isDead = GenerateRandomDeath(current->Info);
        if(isDead){
            // Remove from Parent list of all childs
            HumanListElement* child = current->Info->Childs->First;
            for(int index_child=0; index_child<current->Info->Childs->Size; index_child++){
                child->Info->Parents[current->Info->Gender] = NULL;
                child = child->next;
            }
            
            // If Parents are alive, remove from childs list
            for(int index_parent=0; index_parent < 2; index_parent++){
                Human* parent = current->Info->Parents[index_parent];
                if(parent != NULL){
                    RemoveHumanFromPopulation(parent->Childs, current);
                }
            }

            // If they had a partner, make them single
            if(current->Info->Partner != NULL){
                (current->Info->Partner)->Partner = NULL;
                current->Info->Partner = NULL;
            }

            // Remove from total population
            HumanListElement* tmp = current;
            RemoveHumanFromPopulation(pop, current);
            TotalDeaths++;
            current = tmp->next;
            CheckAnomalies(pop);
        }else{
            current = current->next;
        }
    }
    return TotalDeaths;
}

void BirthdayParty(Population* pop){
    HumanListElement* current = pop->First;
    if(pop->Size<1) return;
    for(int i = 0; i< pop->Size ; i++){
        current->Info->Age++;
        current = current->next;
    }
}

void CheckAnomalies(Population* pop){
    HumanListElement* current = pop->First;
    unsigned int size = 0;
    while(current!=NULL){
        if(current->Info->Dead == 1){
            getchar();
        }
        size++;
        current = current->next;
    }
    if(size != pop->Size){
        printf("Size not match !!\n");
        printf("Calculated size : %d\n", size);
        printf("Stored size : %d\n", pop->Size);
        getchar();
    }
}

int GenerateImmigration(Population* pop){
    int ImmigrationSize = CalculateImmigrationSize(pop);
    for(int i=0; i<ImmigrationSize; i++){
        GenerateRandomHuman();
    }
    return ImmigrationSize;
}

Stats* SimulateNextYear(Population* pop){
    Stats* s = malloc(sizeof(Stats));
    if(pop->Size>0){
        s->InitialPopulationSize = pop->Size;
        CheckAnomalies(pop);
        BirthdayParty(pop);
        GenerateCouple(pop);
        s->NewBorns = GenerateChilds(pop);
        s->DeathsThisYear = GenerateDeath(pop);
        s->FinalPopulationSize = pop->Size;
        s->TotalImmigration = GenerateImmigration(pop);
        if(s->FinalPopulationSize != 0){
            Statistics(pop, s);
        }
        printf("Finished\n");
    }else{
        printf("Everybody is dead\n");
        exit(0);
    }
    return s;
}

void PrintStats(Stats* s, int year){
    printf("\e[1;1H\e[2J");
    printf("--== YEAR %d ==--\n", year);
    if(s->FinalPopulationSize > 0){
        printf("Stats : \n");
        printf("\tPopulation : %d\n", s->FinalPopulationSize);
        printf("\tGrowth : %.2f%%\n", s->Growth);
        printf("\tInitial Population : %d\n", s->InitialPopulationSize);
        printf("\tNew born this year : %d\n", s->NewBorns);
        printf("\tDeaths this year : %d\n", s->DeathsThisYear);
        printf("\tTotal Male : %d\n", s->TotalMale);
        printf("\tTotal Female : %d\n", s->TotalFemale);
        printf("\tTotal Immigration : %d\n", s->TotalImmigration);
        printf("\tMean Age : %.2Lf\n", s->MeanAge);
        printf("\tMean number of childrens per female : %.2Lf\n", s->MeanChildrens);
        printf("\tMost children : %d ( %s %s %d )\n", s->MostChildrens->Childs->Size, s->MostChildrens->FirstName, s->MostChildrens->LastName, s->MostChildrens->Age );
        printf("\tOldest Person : %d %s %s %d\n", s->MostAged->Age, s->MostAged->FirstName, s->MostAged->LastName, s->MostAged->Dead);
        PrintHuman(s->MostAged);
    }
}