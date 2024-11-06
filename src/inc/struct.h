#define GENDER_MALE 0
#define GENDER_FEMALE 1
#define GENDER_ALL 2

typedef struct Human{
    /* Identity */
    char* FirstName;
    char* LastName;
    unsigned char Age;
    unsigned char Gender;
    unsigned char Dead;

    /* Life */
    int Job;
    int Salary;

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