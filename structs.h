#ifndef __STRUCTS__
#define __STRUCTS__ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Some macros for tree printing
#define TABS(depth) for (int x = 0; x < (depth); ++x) {printf("\t");} // print 'depth' tabs
// const int maxLevel = 32; // ~log2(world population)

typedef struct Date {
    int day;        //dd
    int month;      //mm
    int year;       //yyyy
    int empty;
} Date;

// typedef struct sl_node {
//     char *citizenID
    
// }

typedef struct Record {
    char* citizenID;
    char* firstName;
    char* lastName;
    int age;
    char* virus;
    char* country;
    Date vaccDate;     // dd-mm-yyyy
    struct Record* next;
} Record;


typedef struct BloomFilter {
    char* virus;
    int size;
    int k;
    int* bitArray;
    struct BloomFilter* next;
} BloomFilter;

// typedef struct TreeNode {
    //     Date date;
    //     Patient* patientPtr;
    //     struct TreeNode* left;
    //     struct TreeNode* right;
    //     int height;
    // } TreeNode;

    // typedef struct TreeRoot {
    //     char* keyID;        // DiseaseID or countryID
    //     TreeNode* root;     // Pointer to the root
    // } TreeRoot;

    // typedef struct Record {
    //     char* keyID;    // DiseaseID or countryID
    //     TreeRoot* tree; // Pointer to record's tree
    // } Record;

    // typedef struct HashTableBucket {
    //     int recs;               // Number of records in each bucket
    //     void* bucket;           // Memory where records will reside
    //     Patient* patientPtr;
    //     struct HashTableBucket* next;
// } HashTableBucket;

typedef struct SkipNode {
    char* citizenID;
    int level;
    // Array of nexts, one for each level
    // struct SkipNode** next;
} SkipNode;

typedef struct SkipList {
    SkipNode* head;
    // SkipNode* last;
    int maxLevel;
    char* virus;
    struct SkipList* next;
} SkipList;


#endif