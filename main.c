#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "functions.h"

int main(int argc, char **argv) {
    FILE* inputFile;
    // int bucketSize;
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
    
    char* citizenID;
    char* fName;
    char* lName;
    char* country;
    int age;
    char* virus;
    Date vaccDate;
    Record* recordsHead = NULL;
    Record* record;

    BloomFilter* bloomsHead = NULL;
    int k =16;

    // Read from inputFile and create structs
    while (getline(&text, &textSize, inputFile) != -1) {
      //
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
                printf("ERROR IN RECORD %s %s %s %s %d %s \n", citizenID, fName, lName, country, age, virus);
            }
            else {
                vaccDate.day=0;
                vaccDate.month=0;
                vaccDate.year=0;
            }
        }
     //
    //     // Validate that exit date (if given) isn't older than entry date
    //     if (compareDate(entryDate, exitDate) || exitDate.empty) {
            
    //         // Add new record in a linked list
        if (recordsHead == NULL) {
            // printf("FIRST FUCKING RECORD\n");
            recordsHead = createRecord(citizenID, fName, lName, country, age, virus, vaccDate);
        }
        else if (recordsHead != NULL) {
            // Check if record is unique
            if (!checkDuplicate(recordsHead, citizenID, fName, lName, country, age, virus, vaccDate)) {
                record = insertSortedRecord(&recordsHead, citizenID, fName, lName, country, age, virus, vaccDate);
            }
        }
        // Insert in Bloom Filter only vaccinated records
        if (bloomsHead == NULL) {
            // Create empty BloomFilter
            bloomsHead = createBloom(bloomsHead, virus, bloomSize, k);
            // Insert current record in Filter 
            insertInBloom(bloomsHead, citizenID, virus);
        }
        else {
            // Insert in Bloom Filter only vaccinated records
            if (vaccDate.year != 0) {
                if (virusBloomExists(bloomsHead, virus) == 1) {
                    insertInBloom(bloomsHead, citizenID, virus);
                }
                else {
                    bloomsHead = createBloom(bloomsHead, virus, bloomSize, k);
                    insertInBloom(bloomsHead, citizenID, virus);
                }
            }
        }

    //     else
    //         printf("Exit date can't be older than entry date.\nRecord '%s' rejected.\n", recordID);




        free(citizenID);
        free(fName);
        free(lName);
        free(country);
        free(virus);
    }


        free(text);
        char* marika = "1190";
        vaccineStatusBloom(bloomsHead, marika, "Chikungunya");

        // printRecordsList(recordsHead);
        printBloomsList(bloomsHead);
        freeRecordList(recordsHead);
        freeBlooms(bloomsHead);

    // Prepare for receiving commands 
    // size_t inputSize;
    // char* input = NULL;
    // char* command = NULL;

    // // printf("Structs have been constructed. Type a command:\n");

 // while (1) {

    //     getline(&input, &inputSize, stdin);
    //     input[strlen(input)-1] = '\0'; // Cut terminating '\n' from string (with null terminator)

    //     // Get the command
    //     command = strtok(input, " ");

    //     if (!strcmp(command, "/globalDiseaseStats")) {

    //         Date date1;
    //         Date date2;            

    //         // Get [date1]
    //         command = strtok(NULL, " ");
            
    //         if (command) {
                
    //             sscanf(command, "%d-%d-%d", &date1.day, &date1.month, &date1.year);

    //             // Get [date2]
    //             command = strtok(NULL, " ");
                
    //             if (command) {
    //                 sscanf(command, "%d-%d-%d", &date2.day, &date2.month, &date2.year);

    //                 // Call globalDiseaseStats with [date1] [date2]
    //                 globalDiseaseStats(diseaseHT, disease_ht_size, date1, date2);
    //             }
    //             // No [date2], exit command
    //             else {
    //                 printf("No second date typed.\nPlease submit the command again.") ;                   
    //             }
    //         }
    //         // No [date1] [date2]
    //         else {

    //             // Min & max values to include all possible dates
    //             date1.day = 0;
    //             date1.month = 0;
    //             date1.year = 0;
    //             date2.day = 99;
    //             date2.month = 99;
    //             date2.year = 9999;

    //             // Call globalDiseaseStats no time period
    //             globalDiseaseStats (diseaseHT, disease_ht_size, date1, date2);
    //         }
    //     }
    //     else if (!strcmp(command, "/diseaseFrequency")) {

    //         // Get virusName
    //         command = strtok(NULL, " ");
            
    //         if (command) {
                
    //             diseaseID = malloc(strlen(command)+1);
    //             strcpy(diseaseID, command);

    //             // Get 2nd argument
    //             command = strtok(NULL, " ");

    //             if (command) {

    //                 Date date1;
    //                 Date date2;            
                    
    //                 // Get [date1]
    //                 sscanf(command, "%d-%d-%d", &date1.day, &date1.month, &date1.year);

    //                 // Get [date2]
    //                 command = strtok(NULL, " ");
    //                 sscanf(command, "%d-%d-%d", &date2.day, &date2.month, &date2.year);
         
    //                 // Get 3rd argument
    //                 command = strtok(NULL, " ");
                    
    //                 // [country] argument sumbitted
    //                 if (command) {

    //                     // Get [country]
    //                     country = malloc(strlen(command)+1);
    //                     strcpy(country, command);
                        
    //                     // Call diseaseFrequency with [country]
    //                     diseaseFrequency(diseaseHT, disease_ht_size, diseaseID, country, date1, date2);
    //                 }
    //                 // No [country] argument
    //                 else {
    //                     // Call diseaseFrequency without [country]
    //                     country = malloc(strlen(" ")+1);
    //                     strcpy(country, " ");
    //                     diseaseFrequency(diseaseHT, disease_ht_size, diseaseID, country, date1, date2);
    //                 }
    //                 free(country);
    //             }
    //             // No second argument, exit command
    //             else {
    //                 printf("Insufficient arguments typed.\nPlease sumbit the command again.\n");
    //             }
    //             free(diseaseID);
    //         }
    //         // No argument, exit command
    //         else {
    //             printf("No arguments typed.\nPlease sumbit the command again.\n");
    //         }
    //     }
    //     else if (!strcmp(command, "/topk-Diseases")) {

    //         int k;
            
    //         // Get k
    //         command = strtok(NULL, " ");
    //         sscanf(command, "%d", &k);

    //         // Get country
    //         command = strtok(NULL, " ");

    //         if (command) {
                
    //             country = malloc(strlen(command)+1);
    //             strcpy(country, command);

    //             // printf("Asking for top %d diseases recorded in %s.\n", k, country);

    //             Date date1;
    //             Date date2;            

    //             // Get [date1]
    //             command = strtok(NULL, " ");

    //             if (command) {
                    
    //                 sscanf(command, "%d-%d-%d", &date1.day, &date1.month, &date1.year);

    //                 // Get [date2]
    //                 command = strtok(NULL, " ");

    //                 if (command) {
    //                     sscanf(command, "%d-%d-%d", &date2.day, &date2.month, &date2.year);

    //                     // Call topkDiseases with [date1] [date2]
    //                     topkDiseases(countryHT, country_ht_size, country, k, date1, date2);
    //                 }
    //                 // No [date2], exit command
    //                 else {
    //                     printf("No second date typed.\nPlease submit the command again.") ;                   
    //                 }
    //             }
    //             // No [date1] [date2]
    //             else {

    //                 // Min & max values to include all possible dates
    //                 date1.day = 0;
    //                 date1.month = 0;
    //                 date1.year = 0;
    //                 date2.day = 99;
    //                 date2.month = 99;
    //                 date2.year = 9999;

    //                 // Call topkDiseases no time period
    //                 topkDiseases(countryHT, country_ht_size, country, k, date1, date2);
    //             }
    //             free(country);
    //         }
    //         // No country argument, exit command
    //         else {
    //             printf("No country typed.\nPlease sumbit the command again.\n");
    //         }            
    //     }
    //     else if (!strcmp(command, "/topk-Countries")) {

    //         int k;
            
    //         // Get k
    //         command = strtok(NULL, " ");
    //         sscanf(command, "%d", &k);

    //         // Get disease
    //         command = strtok(NULL, " ");

    //         if (command) {
                
    //             diseaseID = malloc(strlen(command)+1);
    //             strcpy(diseaseID, command);

    //             // printf("Asking for top %d countries, where %s has been recorded.\n", k, diseaseID);

    //             Date date1;
    //             Date date2;            

    //             // Get [date1]
    //             command = strtok(NULL, " ");

    //             if (command) {
                    
    //                 sscanf(command, "%d-%d-%d", &date1.day, &date1.month, &date1.year);

    //                 // Get [date2]
    //                 command = strtok(NULL, " ");

    //                 if (command) {
    //                     sscanf(command, "%d-%d-%d", &date2.day, &date2.month, &date2.year);
    //                     // Call topkCountries with [date1] [date2]
    //                     topkCountries(diseaseHT, disease_ht_size, diseaseID, k, date1, date2);
    //                 }
    //                 // No [date2], exit command
    //                 else {
    //                     printf("No second date typed.\nPlease submit the command again.") ;                   
    //                 }
    //             }
    //             // No [date1] [date2]
    //             else {

    //                 // Min & max values to include all possible dates
    //                 date1.day = 0;
    //                 date1.month = 0;
    //                 date1.year = 0;
    //                 date2.day = 99;
    //                 date2.month = 99;
    //                 date2.year = 9999;

    //                 // Call topkCountries no time period
    //                 topkCountries(diseaseHT, disease_ht_size, diseaseID, k, date1, date2);
    //             }
    //             free(diseaseID);
    //         }
    //         // No disease argument, exit command
    //         else {
    //             printf("No disease typed.\nPlease sumbit the command again.\n");
    //         }            
    //     }
    //     else if (!strcmp(command, "/insertPatientRecord")) {

    //         // Get recordID
    //         command = strtok(NULL, " ");
    //         recordID = malloc(strlen(command)+1);
    //         strcpy(recordID, command);

    //         // Get patientFirstName
    //         command = strtok(NULL, " ");
    //         fName = malloc(strlen(command)+1);
    //         strcpy(fName, command);

    //         // Get patientLastName
    //         command = strtok(NULL, " ");
    //         lName = malloc(strlen(command)+1);        
    //         strcpy(lName, command);

    //         // Get diseaseID
    //         command = strtok(NULL, " ");
    //         diseaseID = malloc(strlen(command)+1);
    //         strcpy(diseaseID, command);

    //         // Get country
    //         command = strtok(NULL, " ");
    //         country = malloc(strlen(command)+1);
    //         strcpy(country, command);

    //         // Get entryDate
    //         command = strtok(NULL, " ");
    //         sscanf(command, "%d-%d-%d", &entryDate.day, &entryDate.month, &entryDate.year);

    //         // Get exitDate (or "-")
    //         command = strtok(NULL, " ");

    //         // No exitDate
    //         if (command == NULL){
    //             exitDate.empty = 1;
    //         }
    //         // "-" 
    //         else if (command[0] == '-' ) {
    //             exitDate.empty = 1;
    //         }
    //         // Proper exitDate
    //         else {
    //             sscanf(command, "%d-%d-%d", &exitDate.day, &exitDate.month, &exitDate.year);
    //             exitDate.empty = 0;
    //         }
           
    //         // Add new record in the linked list
    //         last = patientAppend (head, recordID, fName, lName, diseaseID, country, entryDate, exitDate);

    //         // Check if recordID is unique; if not, exit the app
    //         recordExists(head, recordID);

    //         // Add new record in country- and disease-hashtables
    //         countryHashResult = hashFunction(country_ht_size, country);
    //         diseaseHashResult = hashFunction(disease_ht_size, diseaseID);
    //         insertRecord(countryHT, countryHashResult, bucketSize, country, last);
    //         insertRecord(diseaseHT, diseaseHashResult, bucketSize, diseaseID, last);
    //         printf("Record added\n");
    //         free(recordID);
    //         free(fName);
    //         free(lName);
    //         free(diseaseID);
    //         free(country);
    //     }
    //     else if (!strcmp(command, "/recordPatientExit")) {

    //         // Get recordID
    //         command = strtok(NULL, " ");

    //         if (command) {

    //             recordID = malloc(strlen(command)+1);
    //             strcpy(recordID, command);

    //             Date newDate;

    //             // Get exitDate
    //             command = strtok(NULL, " ");
                
    //             if (command) {
    //                 sscanf(command, "%d-%d-%d", &newDate.day, &newDate.month, &newDate.year);

    //                 // Call recordPatientExit
    //                 recordPatientExit(head, recordID, newDate);
    //             }
    //             // No exitDate 
    //             else {
    //                 printf("No exitDate typed.\nPlease submit the command again\n");
    //             }
    //             free(recordID);
    //         }
    //         // No recordID
    //         else {
    //             printf("No recordID typed.\nPlease submit the command again\n");
    //         }
    
    //     }
    //     else if (!strcmp(command, "/numCurrentPatients")) {

    //         command = strtok(NULL, " ");
            
    //         // Get [disease]
    //         if (command) {
    //             diseaseID = malloc(strlen(command)+1);
    //             strcpy(diseaseID, command);

    //             // printf("Disease typed: %s\n", diseaseID);

    //             // Call numCurrentPatients with [disease]
    //             numCurrentPatients(diseaseHT, disease_ht_size, diseaseID);
    //             free(diseaseID);
    //         }
    //         // No [disease]]
    //         else {
    //             // Call numCurrentPatientsAll
    //             numCurrentPatientsAll(diseaseHT, disease_ht_size);
    //         }

    //     }
    //     else if (!strcmp(command, "/exit")) {

    //         // Deallocate memory
    //         free(text);
    //         free(input);
    //         freeRecordList(head);

    //         freeHashTable(countryHT, country_ht_size);
    //         freeHashTable(diseaseHT, disease_ht_size);

            fclose(inputFile);
    //         printf("exiting\n");

    //         return 1;
    //     }
    //     else {
    //         printf("Command '%s' is unknown\n", command);
    //         printf("Please type a known command:\n");
    //     }
// }

    fflush(stdout);

    return 0;
}