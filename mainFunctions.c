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
    head = virusSkipExists(head, virus);
    Record* record = searchSkipList(head, citizenID);
    if (record) {
        printf("VACCINATED ON %d-%d-%d\n", record->vaccDate.day, record->vaccDate.month, record->vaccDate.year);
    }
    else {
        printf("NOT VACCINATED\n");
        return;
    }
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
            printf("%s ", current->virus);
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
    int vaccTotal = 0;
    int nonVaccTotal = 0;
    float percentage = 0;
    Date dateZero;
    Date dateInf;
    dateZero.day = 0;
    dateZero.month = 0;
    dateZero.year = 0;
    dateInf.day = 99;
    dateInf.month = 99;
    dateInf.year = 9999;    
    
    // Country optional argument NOT passed
    if (!country) {        
        State* currentState = stateHead;
        while (currentState) {
            country = currentState->name;
            vaccined = searchCountrySkipList(skipVaccHead, country, date1, date2);
            vaccTotal = searchCountrySkipList(skipVaccHead, country, dateZero, dateInf);
            nonVaccTotal = searchCountrySkipList(skipNonVaccHead, country, dateZero, dateInf);
            // Check if denominator is zero
            if (vaccTotal == 0 && nonVaccTotal == 0) {
                percentage = 0;
            }
            else {
                percentage = ( (float)vaccined / (float)(vaccTotal + nonVaccTotal) ) * 100;
            }
            printf("%s %d %.2f%%\n", country, vaccined, percentage);
            currentState = currentState->next;
        }
    }
    // Country argument passed
    else {
        vaccined = searchCountrySkipList(skipVaccHead, country, date1, date2);
        vaccTotal = searchCountrySkipList(skipVaccHead, country, dateZero, dateInf);
        nonVaccTotal = searchCountrySkipList(skipNonVaccHead, country, dateZero, dateInf);
        // Check if denominator is zero
        if (vaccTotal == 0 && nonVaccTotal == 0) {
            percentage = 0;
        }
        else {
            percentage = ( (float)vaccined / (float)(vaccTotal + nonVaccTotal) ) * 100;
        }
        printf("%s %d %.2f%%\n", country, vaccined, percentage);
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

// Find if Date x lies between Dates a, b
int isBetweenDates (Date a, Date x, Date b) {
    if ( (compareDate(a, x) == 1 || compareDate(a, x) == -1) && (compareDate(x, b) == 1 || compareDate(x, b) == -1) ) {
        return 1;
    }
    else {
        return 0;
    }
}