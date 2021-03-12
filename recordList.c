#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "functions.h"

// Create new linked list record
Record* createRecord (char* citizenID, char* fName, char* lName, char* country, int age, char* virus,  Date vaccDate) {
    Record* head;
    head = malloc(sizeof(Record));

    head->citizenID = malloc(strlen(citizenID)+1);
    strcpy(head->citizenID, citizenID);

    head->firstName = malloc(strlen(fName)+1);
    strcpy(head->firstName, fName);
    
    head->lastName = malloc(strlen(lName)+1);
    strcpy(head->lastName, lName);

    head->country = malloc(strlen(country)+1);
    strcpy(head->country, country);

    head->age = age;

    head->virus = malloc(strlen(virus)+1);
    strcpy(head->virus, virus); 

    head->vaccDate = vaccDate;

    head->next = NULL;

    return head;
}

// Insert record to linked list in a sorted manner
Record* insertSortedRecord (Record** head, char* citizenID, char* fName, char* lName, char* country, int age, char* virus, Date vaccDate) {
    Record* newNode;
    Record* current;

    newNode = malloc(sizeof(Record));
    newNode->next = NULL;

    newNode->citizenID = malloc(strlen(citizenID)+1);
    strcpy(newNode->citizenID, citizenID);

    newNode->firstName = malloc(strlen(fName)+1);
    strcpy(newNode->firstName, fName);

    newNode->lastName = malloc(strlen(lName)+1);
    strcpy(newNode->lastName, lName);

    newNode->country = malloc(strlen(country)+1);
    strcpy(newNode->country, country);

    newNode->age = age;
    
    newNode->virus = malloc(strlen(virus)+1);
    strcpy(newNode->virus, virus);

    newNode->vaccDate = vaccDate;

    // Check if to insert on head
    if (!*head || (strcmp((*head)->citizenID, citizenID)) > 0) {
        newNode->next = *head;
        *head = newNode;
    }
    // Move across list
    else {
        current = *head;
        while ((current->next) && (strcmp(current->next->citizenID, citizenID) <= 0)) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    return newNode;
}

// Validate that record (for same virus) doesn't exist
int checkDuplicate (Record* head, char* citizenID, char* fName, char* lName, char* country, int age, char* virus, Date vaccDate) {  
    while (head) {
        // Check if same citizenID exists
        if (!strcmp(head->citizenID, citizenID)) {
            // Check if name, country, age are the same
            if (!strcmp(head->firstName, fName) && !strcmp(head->lastName, lName) && !strcmp(head->country, country) && (head->age == age)) {
                // Check if virus is the same
                if (!strcmp(head->virus, virus)) {
                    printf("ERROR IN RECORD %s %s %s %s %d %s \n", citizenID, fName, lName, country, age, virus);
                    return 1;
                }
                return 0;
            }
            else {
                printf("ERROR IN RECORD %s %s %s %s %d %s \n", citizenID, fName, lName, country, age, virus);            
                return 1;                
            }
        }
        head = head->next;
    }
    return 0;
}

// Print linked list of records
void printRecordsList (Record* record) {
    while (record) {
        printf("List record: %s %s %s %s %d %s", record->citizenID, record->firstName, record->lastName, record->country, record->age, record->virus);
        if(record->vaccDate.year)
            printf(" %d-%d-%d", record->vaccDate.day, record->vaccDate.month, record->vaccDate.year);
        printf("\n");
        record = record->next;
    }
}

// Free memory allocated for linked list 
void freeRecordList(Record* head) {
    Record* current = head;
    Record* tmp;

    while (current) {
        tmp = current;
        current = current->next;
        free(tmp->citizenID);
        free(tmp->firstName);
        free(tmp->lastName);
        free(tmp->country);
        free(tmp->virus);
        free(tmp);
    }
}