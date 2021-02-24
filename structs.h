#ifndef __STRUCTS__
#define __STRUCTS__ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Some macros for tree printing
#define TABS(depth) for (int x = 0; x < (depth); ++x) {printf("\t");} // print 'depth' tabs

typedef struct Date {
    int day;        //dd
    int month;      //mm
    int year;       //yyyy
    int empty;
} Date;

typedef struct sl_node {
    char *citizenID
    
}



typedef struct Patient {
    char* recordID;
    char* patientFirstName;
    char* patientLastName;
    char* diseaseID;
    char* country;
    Date entryDate;     // dd-mm-yyyy
    Date exitDate;     // dd-mm-yyyy
    struct Patient* next;
} Patient;

typedef struct TreeNode {
    Date date;
    Patient* patientPtr;
    struct TreeNode* left;
    struct TreeNode* right;
    int height;
} TreeNode;

typedef struct TreeRoot {
    char* keyID;        // DiseaseID or countryID
    TreeNode* root;     // Pointer to the root
} TreeRoot;

typedef struct Record {
    char* keyID;    // DiseaseID or countryID
    TreeRoot* tree; // Pointer to record's tree
} Record;

typedef struct HashTableBucket {
    int recs;               // Number of records in each bucket
    void* bucket;           // Memory where records will reside
    struct HashTableBucket* next;
} HashTableBucket;


typedef struct HeapTreeNode {
    char* keyID; // Disease or country
    int count;
    struct HeapTreeNode* parent;
    struct HeapTreeNode* left;
    struct HeapTreeNode* right;
} HeapTreeNode;

typedef struct HeapTree {
    int size;
    HeapTreeNode* root;
    HeapTreeNode* last;
} HeapTree;

typedef struct QueueNode {
    struct QueueNode* next;
    HeapTreeNode* node;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

#endif