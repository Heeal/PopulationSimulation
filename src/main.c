#include <stdlib.h>
#include "inc/struct.h"
#include "inc/dataManagement.h"
#include "inc/WorldGenerator.h"

int main(int argc, char** argv){
    DataManagementInit();
    Population* TotalPopulation = GeneratePopulation(5000);
    PrintPopulation(TotalPopulation);
    return 0;
}