#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "functions.h"

int main(int argc, char **argv) {
    FILE* inputFile;
    int bloomSize;

	// Scan command line arguments
    if (argc != 5) {
        fprintf(stderr, "Error: 4 parameters are required.\n");
        exit(EXIT_FAILURE);
    }
	for (int i=0; i<argc; ++i) {
		if (!strcmp("-c", argv[i])) {
            inputFile = fopen(argv[i+1], "r");
            if(inputFile == NULL) {
                fprintf(stderr, "Cannot open file: %s\n", argv[i+1]);
                return 1;
            }            
        }
		else if (!strcmp("-b", argv[i])) {
			bloomSize = atoi(argv[i+1]);
			if (bloomSize <= 0) {
				fprintf(stderr, "Error: Bloom filter size must be a positive number.\n");
				exit(EXIT_FAILURE);
			}
		}
	}

    // Initialize  variables
    size_t textSize;
    char* text = NULL;
    char* token = NULL;
    
    srand(time(NULL));

    char* citizenID;
    char* fName;
    char* lName;
    char* country;
    int age;
    char* virus;
    Date vaccDate;
    State* stateHead = NULL;
    State* state;
    Record* recordsHead = NULL;
    Record* record;
    BloomFilter* bloomsHead = NULL;
    int k =16;
    int recordAccepted = 1;
    SkipList* skipVaccHead = NULL;
    SkipList* skipNonVaccHead = NULL;

        // Read from inputFile and create structs
    while (getline(&text, &textSize, inputFile) != -1) {
        // Record to be inserted in structs, unless faulty
        recordAccepted = 1;

        // Get citizenID
        token = strtok(text, " ");
        citizenID = malloc(strlen(token)+1);
        strcpy(citizenID, token);

        // Get firstName
        token = strtok(NULL, " ");
        fName = malloc(strlen(token)+1);
        strcpy(fName, token);

        // Get lastName
        token = strtok(NULL, " ");
        lName = malloc(strlen(token)+1);        
        strcpy(lName, token);

        // Get country
        token = strtok(NULL, " ");
        country = malloc(strlen(token)+1);
        strcpy(country, token);       

        // Get age
        token = strtok(NULL, " ");
        sscanf(token, "%d", &age);

        // Get virus
        token = strtok(NULL, " ");
        virus = malloc(strlen(token)+1);
        strcpy(virus, token);

        // Check if YES/NO
        token = strtok(NULL, " ");

        // Get vaccine date, if YES
        if(!strcmp("YES", token)){
            token = strtok(NULL, " ");
            sscanf(token, "%d-%d-%d", &vaccDate.day, &vaccDate.month, &vaccDate.year);
        }
        else {
            // If NO, then record should have no date
            if (token = strtok(NULL, " ")) {
                printf("ERROR IN RECORD %s %s %s %s %d %s - Has 'NO' with date %s", citizenID, fName, lName, country, age, virus, token);
                recordAccepted = 0;
            }
            else {
                vaccDate.day=0;
                vaccDate.month=0;
                vaccDate.year=0;
            }
        }

        // Insert records into structures
        if (recordAccepted) {

            // Add country in State linked list
            // Add each country once only
            if (!stateExists(stateHead, country)) {
                state = insertState(&stateHead, country);
            }
            else {
                state = stateExists(stateHead, country);
            }

            // Add record in Record linked list
            // Check if record is unique
            if (!checkDuplicate(recordsHead, citizenID, fName, lName, state, age, virus, vaccDate)) {
                record = insertSortedRecord(&recordsHead, citizenID, fName, lName, state, age, virus, vaccDate);
                recordAccepted = 1;
            }
            else {
                recordAccepted = 0;
            }
            
            // Add vaccinated record in Bloom Filter
            if (vaccDate.year != 0) {
                // Check if we have Bloom Filter for this virus
                if (virusBloomExists(bloomsHead, virus) == 1) {
                    insertInBloom(bloomsHead, citizenID, virus);
                }
                // Create new Bloom Filter for this virus
                else {
                    bloomsHead = createBloom(bloomsHead, virus, bloomSize, k);
                    insertInBloom(bloomsHead, citizenID, virus);
                }
            }      

            // Add record in Skip List
            // Separate structure for vaccined Skip Lists
            if (vaccDate.year != 0) {
                if (virusSkipExists(skipVaccHead, virus) == 1) {
                    insertInSkip(skipVaccHead, record, virus);
                }
                else {
                    skipVaccHead = createList(skipVaccHead, virus);
                    insertInSkip(skipVaccHead, record, virus);
                }
            }
            // Separate structure for non-vaccined Skip Lists
            else {
                if (virusSkipExists(skipNonVaccHead, virus) == 1) {
                    insertInSkip(skipNonVaccHead, record, virus);
                }
                else {
                    skipNonVaccHead = createList(skipNonVaccHead, virus);
                    insertInSkip(skipNonVaccHead, record, virus);
                }        
                
            }
        }
        free(citizenID);
        free(fName);
        free(lName);
        free(country);
        free(virus);
    }

    // Bloom filter test cases
    // vaccineStatusBloom(bloomsHead, "2135", "Nipah"); // Maybe
    // vaccineStatusBloom(bloomsHead, "2135", "Rabies"); // Not
    // vaccineStatusBloom(bloomsHead, "647", "Bunyavirus"); // Maybe
    // vaccineStatusBloom(bloomsHead, "9872", "YellowFever"); // Maybe

    // printStateList(stateHead);
    // printRecordsList(recordsHead);
    // printBloomsList(bloomsHead);
    // printSkipLists(skipVaccHead);
    // printSkipLists(skipNonVaccHead);

    // Prepare for receiving commands 
    size_t inputSize;
    char* input = NULL;
    char* command = NULL;

    printf("Structs have been constructed. Type a command:\n");

    while (1) {

        getline(&input, &inputSize, stdin);
        input[strlen(input)-1] = '\0'; // Cut terminating '\n' from string (with null terminator)

        // Get the command
        command = strtok(input, " ");

        // /vaccineStatusBloom citizenID virusName 
        if (!strcmp(command, "/vaccineStatusBloom")) {

            // Get citizenID
            command = strtok(NULL, " ");
            if (command) {
                citizenID = malloc(strlen(command)+1);
                strcpy(citizenID, command);
            }

            // Get virusName
            command = strtok(NULL, " ");
            if(command) {
                virus = malloc(strlen(command)+1);
                strcpy(virus, command);
            }

            vaccineStatusBloom(bloomsHead, citizenID, virus);
            free(citizenID);
            free(virus);
        }
        // /vaccineStatus citizenID virusName
        // /vaccineStatus citizenID
        else if (!strcmp(command, "/vaccineStatus")) {
            
            // Get cizitenID
            command = strtok(NULL, " ");
            if (command) {
                citizenID = malloc(strlen(command)+1);
                strcpy(citizenID, command);
            }

            // Get virusName
            command = strtok(NULL, " ");
            // If virusName passed, call vaccineStatus function
            if(command) {
                virus = malloc(strlen(command)+1);
                strcpy(virus, command);
                // vaccineStatus(skipVaccHead, citizenID, virus);
                free(virus);
            }
            // If virusName not passed, call vaccineStatusAll function
            else {
                vaccineStatusAll(skipVaccHead, citizenID);
                vaccineStatusAll(skipNonVaccHead, citizenID);
            }
            free(citizenID);            
        }
        else if (!strcmp(command, "/exit")) {

            // Deallocate memory
            free(text);
            free(input);
            
            freeStateList(stateHead);
            freeRecordList(recordsHead);
            freeBlooms(bloomsHead);
            freeSkipLists(skipVaccHead);
            freeSkipLists(skipNonVaccHead);

            fclose(inputFile);
            printf("exiting\n");

            return 1;
        }
        else {
            printf("Command '%s' is unknown\n", command);
            printf("Please type a known command:\n");
        }
    }

    fflush(stdout);

    return 0;
}