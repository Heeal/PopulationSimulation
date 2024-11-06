#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "inc/struct.h"
#include "inc/dataManagement.h"
#include "inc/WorldGenerator.h"

int main(int argc, char** argv){
    DataManagementInit();
    Population* TotalPopulation = GeneratePopulation(1000000);
    
    for(int year = 0; year<1000; year++){
        Stats* s = SimulateNextYear(TotalPopulation);
        PrintStats(s, year);
    }
    return 0;
}