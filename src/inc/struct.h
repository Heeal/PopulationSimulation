#define GENDER_MALE 0
#define GENDER_FEMALE 1
#define GENDER_ALL 2

/* ADN MASK */
#define DNA_GENDER        0
#define DNA_HAIR_COLOR    1
#define DNA_EYES_COLOR    3
#define DNA_MORPHOTYPE    5
#define DNA_BLOODTYPE     7

/* DNA TRAIT */
#define DNA_HAIR_COLOR_BLACK 0b00
#define DNA_HAIR_COLOR_BROWN 0b01
#define DNA_HAIR_COLOR_BLOND 0b10
#define DNA_HAIR_COLOR_RED   0b11

#define DNA_EYES_COLOR_BLACK 0b00
#define DNA_EYES_COLOR_BROWN 0b01
#define DNA_EYES_COLOR_BLUE  0b10
#define DNA_EYES_COLOR_GREEN 0b11

#define DNA_MORPHOTYPE_ENDOMORPH 0b00
#define DNA_MORPHOTYPE_MESOMORPH 0b01
#define DNA_MORPHOTYPE_ECTOMORPH 0b10

#define DNA_BLOOTYPE_O  0b00
#define DNA_BLOOTYPE_A  0b01
#define DNA_BLOOTYPE_B  0b10
#define DNA_BLOOTYPE_AB 0b11

typedef struct DNA{      
    /*
    
Bit:    32-10        09          08           07           06        05     04     03     02     01
    ----------------------------------------------------------------------------------------------------
       NOT USED | BLOODTYPE | BLOODTYPE | MORPHOTYPE | MORPHOTYPE | EYES | EYES | HAIR | HAIR | GENDER |
    ----------------------------------------------------------------------------------------------------


    HAIR : 
        00 => Black
        01 => Brown
        10 => Blond
        11 => Red
    EYES : 
        00 => Black
        01 => Brown
        10 => Blue
        11 => Green

    MORPHOTYPE :
        00 : Endomorph 
        01 : Mesomorph
        10 : Ectomorph

    BLOOD TYPE :
        00 : O
        01 : A
        10 : B
        11 : AB
    */
    short traits;
} DNA;

typedef struct Human{
    /* Identity */
    char* FirstName;
    char* LastName;
    unsigned char Age;
    unsigned char Gender;
    short DNA;
    unsigned char Dead;

    /* Family */
    struct Human* Parents[2];
    struct Human* Partner;
    struct Population* Childs;
} Human;

typedef struct HumanListElement{
    Human* Info;
    struct HumanListElement* next;
    struct HumanListElement* previous;
} HumanListElement;

typedef struct Population{
    struct HumanListElement* First;
    struct HumanListElement* Last;
    unsigned int Size;
} Population;


typedef struct FirstNameDistribution{
    // Array of possible name per gender
    char** FirstNames[2];

    // Law of distribution per gender
    unsigned long long int* Distribution[2];

    //Total counted per gender
    unsigned long long int Total[3];
    
    //Number of distinct names per gender
    unsigned long long int Size[3];
} FirstNameDistribution;

typedef struct LastNameDistribution{
    // Array of possible name per gender
    char** LastNames;
    unsigned long long int Size;
} LastNameDistribution;


typedef struct Stats{
    unsigned int InitialPopulationSize;
    unsigned int FinalPopulationSize;
    unsigned int DeathsThisYear;
    unsigned int NewBorns;
    double Growth;
    unsigned int TotalMale;
    unsigned int TotalFemale;
    unsigned int TotalImmigration;
    long double MeanAge;
    long double MeanChildrens;

    Human* MostAged;
    Human* MostChildrens;
} Stats;