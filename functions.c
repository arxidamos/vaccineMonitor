#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <stdarg.h>
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

// Insert record to linked list  in a sorted manner
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

    // Check if insert on head
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

// Print linked list
void printRecordsList (Record* record) {
    while (record) {
        printf("List record: %s %s %s %s %d %s", record->citizenID, record->firstName, record->lastName, record->country, record->age, record->virus);
        if(record->vaccDate.year)
            printf(" %d-%d-%d", record->vaccDate.day, record->vaccDate.month, record->vaccDate.year);
        printf("\n");
        record = record->next;
    }
}

// Free memory alloc'd for linked list
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Hash aux function djb2
unsigned long djb2(unsigned char *str) {
    unsigned long hash = 5381;
    int c; 
    while ( (c = *str++) ) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

// Hash aux function sdbm
unsigned long sdbm(unsigned char *str) {
    unsigned long hash = 0;
    int c;
    while ( (c = *str++) ) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

// Hash function hash_i
unsigned long hash_i(unsigned char *str, unsigned int i) {
    return djb2(str) + i*sdbm(str) + i*i;
}

BloomFilter* createBloom (BloomFilter* bloomsHead, char* virus, int size, int k) {
    BloomFilter* newBloom;
    newBloom = malloc(sizeof(BloomFilter));
    newBloom->size = size;
    newBloom->k = k;
    newBloom->virus = malloc(strlen(virus)+1);
    strcpy(newBloom->virus, virus); 
    newBloom->bitArray = calloc(size, 1); // malloc size elements of 1 byte each, initialize them to 0
    
    // Put new Bloom in head
    if (bloomsHead) {
        newBloom->next = bloomsHead;
    }
    else {
        newBloom->next = NULL;
    }

    return newBloom;
}

void insertInBloom (BloomFilter* bloomsHead, char* citizenID, char* virus) {
    BloomFilter* current = bloomsHead;
    unsigned char* id = (unsigned char*)citizenID;
    unsigned long hash;
    unsigned int set = 1; // All 0s and leftmost bit=1
    int x;
    // Search for this virus' BF
    while (current) {
        if (!strcmp(current->virus, virus)) {
            //hash klp klp
            for (unsigned int i=0; i<(current->k-1); i++) {
                // Hash the ID
                hash = hash_i(id, i);
                // Get the equivalent bit position that needs to be set to 1
                x = hash%(current->size*8);
                // An int is 32 bits
                // So x/32 is the index in the bitArray
                // And x%32 is the bit's position inside that index
                
                // Shift left the *set* bit
                // printf("Set bit before shift: %d\t", set);
                set = set << (x%32);
                // printf("after shift: %d\n", set);
                current->bitArray[x/32] = current->bitArray[x/32] | set;
                // printf("%d\n", bloomsHead->bitArray[x/32]);
                set = 1;
            } 
            // printf("X= %d\n", x);
            return;
        }
        current = current->next;
    }
}

int virusBloomExists (BloomFilter* bloomsHead, char* virus) {
    BloomFilter* current = bloomsHead;
    while (current) {
        if (!strcmp(current->virus, virus)) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void printBloomsList (BloomFilter* head) {
    BloomFilter* current = head;    
    while (current) {
        printf("BF : %s\n", current->virus);    
        current = current->next;
    }
}

// Free memory alloc'd for Bloom Filters
void freeBlooms(BloomFilter* head) {
    BloomFilter* current = head;
    BloomFilter* tmp;

    while (current) {
        tmp = current;
        current = current->next;
        free(tmp->bitArray);
        free(tmp->virus);
        free(tmp);
    }
}

void vaccineStatusBloom(BloomFilter* head, char* citizenID, char* virus) {
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
                if ((current->bitArray[x/32] & set) != 0) {
                    // printf("x=%d, bitArray[%d/32]=%d, setBit=%d\n", x, x, current->bitArray[x/32], set);

                    // printf("NOT VACCINATED\n");
                    // return;
                }
                else {
                    printf("NOT VACC\n");
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SkipList* createList(SkipList* skipListHead, char* virus) {
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

void insertInSkip(SkipList* skipListHead, Record* record, char* virus) {
    SkipList* skipL = skipListHead;

    while (skipL) {
        if (!strcmp(skipL->virus, virus)) {
            // We are in the right list
            printf("Inserting in list %s\n", skipL->virus);
            SkipNode* current = skipL->head;
            int level = skipL->head->levels - 1;
            int compare;
            // Keep last visited for each level            
            SkipNode* lastVisited[max];
            // for (int i=0; i<max; i++) {
                //     lastVisited[i] = malloc(sizeof(SkipNode));
            // }
            // for (int i=0; i<max; i++) {
            //     lastVisited[i] = NULL;
            // }
            
            // Search Lists' nodes, starting from top level
            while ( (current != NULL) && (level>=0) ) {
                lastVisited[level] = current;
                // No more nodes on this level                
                if (current->next[level] == NULL) {
                    printf("reached level #%d end\n", level);
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
                        printf("Bigger node in level #%d\n", level);
                        level--;
                    }
                    // Keep checkin on this level
                    else {
                        printf("Move on, on same level #%d\n", level);
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
            printf("New node will randomly have %d levels\n", newNode->levels);
            
            // for (int i=0; i<newNode->levels; i++){
            //     newNode->next[i] = NULL;
            // }

            // for (int i=0; i<newNode->levels; i++){
            //     lastVisited[i]->next[i] = NULL;
            // }

                        
            for (int i=0; i<newNode->levels; i++) {
            // for (int i=newNode->levels-1; i>=0; i--) {

                // printf("%dh fora\n", i);
                newNode->next[i] = lastVisited[i]->next[i];
                lastVisited[i]->next[i] = newNode;
            }
            if (newNode->next[0] != NULL) {
                printf("New node put: %s--%s--%s\n\n", lastVisited[0]->citizenID, newNode->citizenID, newNode->next[0]->citizenID);
            }
            else {
                printf("New node put: %s--%s--Null\n\n", lastVisited[0]->citizenID, newNode->citizenID);
            }
            return;
        }
        skipL = skipL->next;    
    }
}

Record* searchSkipLists(SkipList* skipListHead, char* virus, char* citizenID) {
    SkipNode* current = skipListHead->head;
    int level = skipListHead->head->levels - 1;
    int compare;

    while ( (current != NULL) && (level>=0) ) {
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
            if (current->next[level]) {
                current = current->next[level];
            }
            // Unless at level's end
            else {
                level--;
            }
        }
    }
    return NULL;
}

// Free memory alloc'd for skip Lists
void freeSkipLists(SkipList* head) {
    SkipList* currentList = head;
    SkipList* tmpList;
    // SkipNode* currentNode;
    // SkipNode* tmpNode;

    // Iterate through all skip Lists
    while (currentList) {
        tmpList = currentList;
        currentList = currentList->next;

        // Free all nodes of this list
        freeSkipNodes(tmpList);

        // free(tmpList->head->citizenID);
        // free(tmpList->head);
        // printf("Just freed %s.\n", tmpList->virus);
        free(tmpList->virus);
        free(tmpList);
        
    }
}

int virusSkipExists(SkipList* skipListHead, char* virus) {
    SkipList* current = skipListHead;
    while (current) {
        if (!strcmp(current->virus, virus)) {
            printf("Existing list %s\n", current->virus);            
            return 1;
        }
        current = current->next;
    }
    printf("List %s to be added\n", virus);
    return 0;
}

void freeSkipNodes(SkipList* skipList) {
    // Iterate through all levels of this List
        // for (int i=(tmpList->maxLevel-1); i>=0; i--) {
        //     // printf("before!\n");
            
        //     currentNode = tmpList->head;
        //     while (currentNode->next[i]) {
        //         tmpNode = currentNode;
        //         currentNode = currentNode->next[i];

        //         if (tmpNode != tmpList->head) {
        //             free(tmpNode->citizenID);
        //             free(tmpNode);   
        //         }
        //     }
        //     // free(tmpList->head->next[i]);
        // }
        // for (int i=(tmpList->maxLevel-1); i>=0; i--) {
        //     free(tmpList->head->next[i]);
        // }
    // free(tmpList->head->next);

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

void printSkipLists (SkipList* head) {
    SkipList* current = head;    
    while (current) {
        printf("SL : %s\n", current->virus);
        printSkipNodes(current);
        current = current->next;
    }
}

void printSkipNodes(SkipList* skipList) {
    SkipNode* current = skipList->head;
    int height = skipList->head->levels;

    for (int i=height-1; i>=0; i--) {

        while (current) {
            printf("[%s]-->", current->citizenID);
            current = current->next[i];
            // current = current->next[0];

        }
        current = skipList->head;
        printf("[end[%d]]\n", i);
        // printf("[end[%d]]\n", 0);


    }
}

int getHeight(int maximum) {
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
