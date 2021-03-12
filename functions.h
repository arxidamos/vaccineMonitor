#ifndef __FUNCTIONS__
#define __FUNCTIONS__ 
#include "structs.h"

// recordList.c
Record* createRecord (char* citizenID, char* fName, char* lName, char* country, int age, char* virus,  Date vaccDate);
Record* insertSortedRecord (Record** head, char* citizenID, char* fName, char* lName, char* country, int age, char* virus, Date vaccDate);
int checkDuplicate (Record* head, char* citizenID, char* fName, char* lName, char* country, int age, char* virus, Date vaccDate);
void printRecordsList (Record* record);
void freeRecordList (Record* head);

// bloomFilter.c
unsigned long djb2(unsigned char *str);
unsigned long sdbm(unsigned char *str);
unsigned long hash_i(unsigned char *str, unsigned int i);
BloomFilter* createBloom (BloomFilter* bloomsHead, char* virus, int size, int k);
void insertInBloom (BloomFilter* bloomsHead, char* citizenID, char* virus);
int virusBloomExists (BloomFilter* bloomsHead, char* virus);
void printBloomsList (BloomFilter* head);
void freeBlooms(BloomFilter* head);

// skipList.c
SkipList* createList(SkipList* skipListHead, char* virus);
void insertInSkip(SkipList* skipListHead, Record* record, char* virus);
Record* searchSkipLists(SkipList* skipListHead, char* virus, char* citizenID);
int virusSkipExists(SkipList* skipListHead, char* virus);
int getHeight(int maximum);
void printSkipLists (SkipList* head);
void printSkipNodes(SkipList* skipList);
void freeSkipLists(SkipList* head);
void freeSkipNodes(SkipList* skipList);

// mainFunctions.c
// Command functions
void vaccineStatusBloom(BloomFilter* head, char* citizenID, char* virus);

// Auxiliary functions

#endif