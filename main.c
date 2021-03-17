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

    // Initialize variables
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
        }

        if (recordAccepted) {

            // Add vaccinated record in Bloom Filter
            if (vaccDate.year != 0) {
                // Check if we have Bloom Filter for this virus
                if (virusBloomExists(bloomsHead, virus)) {
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
                if (virusSkipExists(skipVaccHead, virus)) {
                    insertInSkip(skipVaccHead, record, virus);
                }
                else {
                    skipVaccHead = createList(skipVaccHead, virus);
                    insertInSkip(skipVaccHead, record, virus);
                }
            }
            // Separate structure for non-vaccined Skip Lists
            else {
                if (virusSkipExists(skipNonVaccHead, virus)) {
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

                // Get virusName
                command = strtok(NULL, " ");
                if(command) {
                    virus = malloc(strlen(command)+1);
                    strcpy(virus, command);
                    vaccineStatusBloom(bloomsHead, citizenID, virus);
                    free(virus);
                }
                else {
                    printf("Please enter a virus name\n");
                }
                free(citizenID);
            }
            else {
                printf("Please enter a citizenID and a virus name\n");
            }
        }
        // /vaccineStatus citizenID [virusName]
        else if (!strcmp(command, "/vaccineStatus")) {
            
            // Get cizitenID
            command = strtok(NULL, " ");
            if (command) {
                citizenID = malloc(strlen(command)+1);
                strcpy(citizenID, command);

                // Check that citizenID is valid
                if (checkExistence(recordsHead, citizenID)) {
                    
                    // Get virusName
                    command = strtok(NULL, " ");
                    // Call vaccineStatus function
                    if(command) {
                        virus = malloc(strlen(command)+1);
                        strcpy(virus, command);
                        vaccineStatus(skipVaccHead, citizenID, virus);
                        free(virus);
                    }
                    // Call vaccineStatusAll function
                    else {
                        vaccineStatusAll(skipVaccHead, citizenID);
                        vaccineStatusAll(skipNonVaccHead, citizenID);
                    }
                }
                else {
                    printf("Please enter a valid citizenID\n");
                }
                free(citizenID);
            }
            else {
                printf("Please enter a citizenID\n");
            }
        }
        // /populationStatus [country] virusName date1 date2
        else if (!strcmp(command, "/populationStatus")) {
            // find, for each country, and for virusName, the count of people that are vaccined and the fraction: vaccined/ vaccined + nonVaccined
            
            // Get country OR virusName
            command = strtok(NULL, " ");
            if (command) {
                // 1st argument: existing country
                if (stateExists(stateHead, command)){
                    country = malloc(strlen(command)+1);
                    strcpy(country, command);

                    // Get virusName
                    command = strtok(NULL, " ");
                    if (command) {
                        // 2nd argument: existing virusName
                        if (virusSkipExists(skipVaccHead, command) || virusSkipExists(skipNonVaccHead, command)) {                        
                            virus = malloc(strlen(command)+1);
                            strcpy(virus, command);
                    
                            Date date1;
                            Date date2;

                            // Get date1
                            command = strtok(NULL, " ");
                            if (command) {
                                // 3rd argument: date1
                                sscanf(command, "%d-%d-%d", &date1.day, &date1.month, &date1.year);

                                // Get date2
                                command = strtok(NULL, " ");
                                if (command) {
                                    // 4rth argument: date2
                                    sscanf(command, "%d-%d-%d", &date2.day, &date2.month, &date2.year);

                                    // Call function
                                    SkipList* vaccSkipList = virusSkipExists(skipVaccHead, virus);
                                    SkipList* nonVaccSkipList = virusSkipExists(skipNonVaccHead, virus);
                                    populationStatus(vaccSkipList, nonVaccSkipList, country, stateHead, date1, date2);
                                }
                                // No date2, exit command
                                else {
                                    printf("ERROR:\nNo second date typed. Please submit the command again.\n");
                                }
                            }
                            // No 3rd and 4th arguments
                            else {
                                // Min & max values to include all possible dates
                                date1.day = -1;
                                date1.month = -1;
                                date1.year = -1;
                                date2.day = 99;
                                date2.month = 99;
                                date2.year = 9999;

                                // Call function
                                SkipList* vaccSkipList = virusSkipExists(skipVaccHead, virus);
                                SkipList* nonVaccSkipList = virusSkipExists(skipNonVaccHead, virus);
                                populationStatus(vaccSkipList, nonVaccSkipList, country, stateHead, date1, date2);
                            }
                            free(virus);
                        }
                        else {
                            printf("Please enter an existing virus name\n");
                        }
                    }
                    else {
                        printf("Please enter a virus name\n");
                    }
                    free(country);
                }
                // 1st argument: existing virusName
                else if (virusSkipExists(skipVaccHead, command) || virusSkipExists(skipNonVaccHead, command)) {                    
                    // No optional country argument passed
                    virus = malloc(strlen(command)+1);
                    strcpy(virus, command);
                    printf("Ok, no country, only virus %s given\n", virus);

                    Date date1;
                    Date date2;
                    country = NULL;

                    // Get date1
                    command = strtok(NULL, " ");
                    if (command) {
                        // 3rd argument: date1
                        sscanf(command, "%d-%d-%d", &date1.day, &date1.month, &date1.year);

                        // Get date2
                        command = strtok(NULL, " ");
                        if (command) {
                            // 4rth argument: date2
                            sscanf(command, "%d-%d-%d", &date2.day, &date2.month, &date2.year);

                            // Call function
                            SkipList* vaccSkipList = virusSkipExists(skipVaccHead, virus);
                            SkipList* nonVaccSkipList = virusSkipExists(skipNonVaccHead, virus);
                            populationStatus(vaccSkipList, nonVaccSkipList, country, stateHead, date1, date2);
                          }
                        // No date2, exit command
                        else {
                            printf("ERROR:\nNo second date typed. Please submit the command again.\n");
                        }
                    }
                    // No 3rd and 4th arguments
                    else {
                        // Min & max values to include all possible dates
                        date1.day = 0;
                        date1.month = 0;
                        date1.year = 0;
                        date2.day = 99;
                        date2.month = 99;
                        date2.year = 9999;

                        // Call function
                        SkipList* vaccSkipList = virusSkipExists(skipVaccHead, virus);
                        SkipList* nonVaccSkipList = virusSkipExists(skipNonVaccHead, virus);
                        populationStatus(vaccSkipList, nonVaccSkipList, country, stateHead, date1, date2);
                    }

                    free(virus);
                }
                // 1st argument: neither country nor virus
                else {
                    printf("Please enter an existing country or virus name\n");
                }
            }
            else {
                printf("More arguments needed. Proper command structure:\n");
                printf("** /populationStatus [country] virusName date1 date2 **\n");
            }
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