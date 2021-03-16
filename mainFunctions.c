#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "functions.h"

// Check if citizenID belongs to virus' Bloom Filter
void vaccineStatusBloom (BloomFilter* head, char* citizenID, char* virus) {
    BloomFilter* current = head;
    unsigned char* id = (unsigned char*)citizenID;
    unsigned long hash;
    unsigned int set = 1; // All 0s and leftmost bit=1
    int x;

    while (current) {
        if (!strcmp(current->virus, virus)) {
            // Hash the id to get the bits we need to check
            for (unsigned int i=0; i<(current->k-1); i++) {
                // Hash the ID
                hash = hash_i(id, i);
                // Get the equivalent bit position that may be 1
                x = hash%(current->size*8);
                // An int is 32 bits
                // So x/32 is the index in the bitArray
                // And x%32 is the bit's position inside that index
                
                // Shift left the *set* bit
                set = set << (x%32);
                // printf("after shift: %d\n", set);

                // printf("Comparing set and bitarray bit: %d\n", result);
                // Check if bitArray has *1* in the same spot
                if ((current->bitArray[x/32] & set) == 0) {
                    // printf("x=%d, bitArray[%d/32]=%d, setBit=%d\n", x, x, current->bitArray[x/32], set);
                    printf("NOT VACCINATED\n");
                    return;
                }
                set = 1;                
            }
            printf("MAYBE\n");
            return;
        }
        current = current->next;
    }
    printf("There is no Bloom Filter for the virus name you inserted.\n");
    return;
}

// Check if citizenID belongs to virus' Skip List 
void vaccineStatus (SkipList* head, char* citizenID, char* virus) {
    SkipList* current = head;
    Record* record = NULL;

    // Iterate through Skip Lists
    while (current) {
        if (virusSkipExists(current, virus)) {
            if (searchSkipList(current, citizenID)) {
                record = searchSkipList(current, citizenID);
                printf("VACCINATED ON %d-%d-%d\n", record->vaccDate.day, record->vaccDate.month, record->vaccDate.year);
                return;
            }
        }
        current = current->next;
    }
    printf("NOT VACCINATED\n");
    return;
}

// Display all vacc and non-vacc occurences for citizenID
void vaccineStatusAll (SkipList* head, char* citizenID) {
    SkipList* current = head;
    Record* record = NULL;

    // Iterate through Skip Lists
    while (current) {
        record =  searchSkipList(current, citizenID);
        if(record){
            // Print virus
            printf("%s ", record->virus);
            // Print YES dd-mm-yyyy
            if (record->vaccDate.year != 0) {
                printf("YES %d-%d-%d\n", record->vaccDate.day, record->vaccDate.month, record->vaccDate.year);
            }
            // Print NO
            else {
                printf("NO\n");
            }
        }
        current = current->next;
    }
    return;
}

// Display country's vaccined total and percentage of citizens for virus
void populationStatus (SkipList* skipVaccHead, SkipList* skipNonVaccHead, char* country, State* stateHead, Date date1, Date date2) {
    
    int vaccined = 0;
    int nonVaccined = 0;
    float percentage = 0;
    
    // Country optional argument NOT passed
    if (!country) {        
        State* currentState = stateHead;
        while (currentState) {
            country = currentState->name;
            vaccined = searchCountrySkipList(skipVaccHead, country, date1, date2);
            nonVaccined = searchCountrySkipList(skipNonVaccHead, country, date1, date2);
            printf("%s %d %d\n", country, vaccined, nonVaccined);

            currentState = currentState->next;
        }
    }
    // Country argument passed
    else {
        vaccined = searchCountrySkipList(skipVaccHead, country, date1, date2);
        nonVaccined = searchCountrySkipList(skipNonVaccHead, country, date1, date2);
        printf("%s %d %d\n", country, vaccined, nonVaccined);
    }
    return;
}

// Compare dates - Returns : "1", a older than b [a < b] | "0", a newer than b [a > b] | "-1": [a = b]
int compareDate (Date a, Date b) {
    if (a.year != b.year)
        return a.year < b.year;
    else if (a.month != b.month)
        return a.month < b.month;
    else if (a.day != b.day)
        return a.day < b.day;
    else {
        // Dates are the same
        return -1;
    }
}