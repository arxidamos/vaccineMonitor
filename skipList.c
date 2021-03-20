#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "functions.h"

// Create new Skip List for new virus. Keep vacc lists and non-vacc lists together
SkipList* createList (SkipList* skipListHead, char* virus) {
    SkipList* newList;
    newList = malloc(sizeof(SkipList));
    // newList->head = calloc(1, sizeof(SkipNode));
    newList->head = malloc(sizeof(SkipNode));

    newList->head->levels = max;
    newList->head->citizenID = malloc(strlen("@")+1);
    strcpy(newList->head->citizenID, "@");

    for (int i=0; i<newList->head->levels; i++){
        newList->head->next[i] = NULL;
    }

    newList->maxLevel = max; // ~log2(world population)
    newList->virus = malloc(strlen(virus)+1);
    strcpy(newList->virus, virus);

    // newList->last = malloc(sizeof(SkipNode));
    // newList->head->citizenID = malloc(strlen("@")+1);
    // strcpy(newList->head->citizenID, "@");

    // newList->head->citizenID = "@";
    // newList->last->citizenID = "z";

    // newList->head->next = malloc(sizeof(SkipNode*)*(newList->maxLevel));
    // // Initialize all levels' next pointers
    // for (int i=0; i<newList->maxLevel; i++) {
    // // for (int i=0; i<=0; i++) {

    //     // newList->head->next[i] = malloc(sizeof(SkipNode));
    //     newList->head->next[i] = NULL;
    // }

    // Put new skipList in head
    if (skipListHead) {
        // printf("new skipList for %s\n", virus);
        newList->next = skipListHead;
    }
    else {
        // printf("1st skipList for %s\n", virus);
        newList->next = NULL;
    }
    return newList;
}

// Insert new element in Skip List
void insertInSkip (SkipList* skipListHead, Record* record, char* virus) {
    SkipList* skipL = skipListHead;

    while (skipL) {
        if (!strcmp(skipL->virus, virus)) {
            // We are in the right list
            // printf("Inserting in list %s\n", skipL->virus);
            SkipNode* current = skipL->head;
            int level = skipL->head->levels - 1;
            int compare;
            // Keep last visited for each level            
            SkipNode* lastVisited[max];
            
            // Search Lists' nodes, starting from top level
            while ( (current != NULL) && (level>=0) ) {
                lastVisited[level] = current;
                // No more nodes on this level                
                if (current->next[level] == NULL) {
                    // printf("reached level #%d end\n", level);
                    level--;
                }
                // More nodes on this level
                else {                
                    compare = strcmp(current->next[level]->citizenID, record->citizenID);
                    // ID already exists
                    if (!compare) {
                        return;
                    }
                    // Reached level's bigger ID
                    else if (compare>0) {
                        // printf("Bigger node in level #%d\n", level);
                        level--;
                    }
                    // Keep checkin on this level
                    else {
                        // printf("Move on, on same level #%d\n", level);
                        current = current->next[level];
                    }
                }
            }

            // Insert new node
            SkipNode* newNode = malloc(sizeof(SkipNode));
            newNode->citizenID = malloc(strlen(record->citizenID)+1);
            strcpy(newNode->citizenID, record->citizenID);
            newNode->record = record;
            newNode->levels = getHeight(max);
            // printf("New node will randomly have %d levels\n", newNode->levels);
                                 
            for (int i=0; i<newNode->levels; i++) {
            // for (int i=newNode->levels-1; i>=0; i--) {

                // printf("%dh fora\n", i);
                newNode->next[i] = lastVisited[i]->next[i];
                lastVisited[i]->next[i] = newNode;
            }
            // if (newNode->next[0] != NULL) {
            //     printf("New node put: %s--%s--%s\n\n", lastVisited[0]->citizenID, newNode->citizenID, newNode->next[0]->citizenID);
            // }
            // else {
            //     printf("New node put: %s--%s--Null\n\n", lastVisited[0]->citizenID, newNode->citizenID);
            // }
            return;
        }
        skipL = skipL->next;    
    }
}

// Check for citizenID in Skip List
Record* searchSkipList (SkipList* skipListHead, char* citizenID) {
    SkipNode* current = skipListHead->head;
    int level = skipListHead->head->levels - 1;
    int compare;
    while ( (current != NULL) && (level>=0) ) {
        // No more nodes on this level
        if (current->next[level] == NULL) {
            level--;
        }
        // More nodes on this level
        else {
            compare = strcmp(current->next[level]->citizenID, citizenID);
            // Found, return record pointer
            if (!compare) {
                return current->next[level]->record;
            }
            // Reached level's bigger ID
            else if (compare>0) {
                level--;
            }
            // Keep checkin on this level
            else {
                current = current->next[level];
            }
        }
    }
    return NULL;
}

// Check for country in Skip List
int searchCountrySkipList (SkipList* skipListHead, char* country, Date date1, Date date2) {   
    // No Skip List for this virus
    if (!skipListHead) {
        return 0;
    }

    SkipNode* current = skipListHead->head;
    int count = 0;

    // Can't make use of SkipList's citizenID-based level-iteration
    while (current->next[0]) {
        if (!strcmp(current->next[0]->record->country->name, country)) {
            if (isBetweenDates(date1, current->next[0]->record->vaccDate, date2)) {
                count++;
            }
        }
        current = current->next[0];
    }
    return count;
}

// Check for country in Skip List, per age category
int* searchCountryByAge (SkipList* skipListHead, char* country, Date date1, Date date2) {
    // int count0_20 = 0;
    // int count20_40 = 0;
    // int count40_60 = 0;
    // int count60_ = 0;
    // Store each age category's counter under the corresponding array index
    int* vaccArray = malloc(4*sizeof(int));
    for (int i=0; i<4; i++) {
        vaccArray[i] = 0;
    }    

    // No Skip List for this virus
    if (!skipListHead) {
        return vaccArray;
    }

    SkipNode* current = skipListHead->head;

    // Can't make use of SkipList's citizenID-based level-iteration
    while (current->next[0]) {  
        if (!strcmp(current->next[0]->record->country->name, country)) {
            if (isBetweenDates(date1, current->next[0]->record->vaccDate, date2)) {
                // Increment the proper age category counter
                if (current->next[0]->record->age > 0 && current->next[0]->record->age < 20) {
                    vaccArray[0]++;
                }
                else if (current->next[0]->record->age >= 20 && current->next[0]->record->age < 40) {
                    vaccArray[1]++;
                }
                else if (current->next[0]->record->age >= 40 && current->next[0]->record->age < 60) {
                    vaccArray[2]++;
                }
                else if (current->next[0]->record->age >= 60) {
                    vaccArray[3]++;
                }
            }
        }
        current = current->next[0];
    }
    return vaccArray;
}

// Check if Skip List for this virus exists
SkipList* virusSkipExists (SkipList* skipListHead, char* virus) {
    SkipList* current = skipListHead;
    while (current) {
        if (!strcmp(current->virus, virus)) {
            // printf("Existing list %s\n", current->virus);
            return current;
        }
        current = current->next;
    }
    // printf("List %s to be added\n", virus);
    return NULL;
}

// Create random maximum height for new Skip Nodes
int getHeight (int maximum) {
    // Generate random new height: 
    int result;
    int height=1;
    for (int i=1; i<maximum; i++) {
        result = rand() % RAND_MAX;
        if (result < (RAND_MAX/2)) {
            height++;
        }
    }
    return height;
}

// Print lists (vacc and non-vacc) of Skip Lists
void printSkipLists (SkipList* head) {
    SkipList* current = head;
    while (current) {
        printf("SL : %s\n", current->virus);
        printSkipNodes(current);
        current = current->next;
    }
}

// Print Skip nodes inside Skip Lists
void printSkipNodes (SkipList* skipList) {
    // int height = skipList->head->levels;
    // for (int i=height-1; i>=0; i--) {
    //     while (current) {
    //         // printf("[%s]-->", current->citizenID);
    //         current = current->next[i];
    //         // current = current->next[0];
    //     }
    //     current = skipList->head;
    //     // printf("[end[%d]]\n", i);
    // }

    // No Skip List for this virus
    if (!skipList->head) {
        return;
    }

    // Start from 1st node after head
    SkipNode* current = skipList->head->next[0];
    while (current) {
        printf("%s %s %s %s %d\n",
        current->citizenID,
        current->record->firstName,
        current->record->lastName,
        current->record->country->name,
        current->record->age);
        current = current->next[0];
    }
}

// Free memory allocated for Skip Lists
void freeSkipLists (SkipList* head) {
    SkipList* currentList = head;
    SkipList* tmpList;

    // Iterate through all skip Lists
    while (currentList) {
        tmpList = currentList;
        currentList = currentList->next;

        // Free all nodes of this list
        freeSkipNodes(tmpList);

        // printf("Just freed %s.\n", tmpList->virus);
        free(tmpList->virus);
        free(tmpList);
    }
}

// Free moemory allocated for Skip Nodes inside Skip Lists
void freeSkipNodes (SkipList* skipList) {
    SkipNode* current = skipList->head;
    SkipNode* tmp;

    while (current) {
        tmp = current;
        current = current->next[0];
        free(tmp->citizenID);
        free(tmp);
    }
    return;
}