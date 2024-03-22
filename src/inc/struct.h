#define GENDER_MALE 0
#define GENDER_FEMALE 1
#define GENDER_ALL 2

typedef struct Human{
    /* Identity */
    char* FirstName;
    char* LastName;
    unsigned char Age;
    unsigned char Gender;

    /* Life */
    int Job;
    int Salary;

    /* Family */
    struct Human* Father;
    struct Human* Mother;
    struct Human* Partner;
    struct Population* Childs;
} Human;

typedef struct HumanListElement{
    Human* Info;
    struct HumanListElement* next;
} HumanListElement;

typedef struct Population{
    struct HumanListElement* First;
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