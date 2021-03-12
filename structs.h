#ifndef __STRUCTS__
#define __STRUCTS__ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define max 32

typedef struct Date {
    int day;        //dd
    int month;      //mm
    int year;       //yyyy
    int empty;
} Date;

typedef struct State {
    char* name;
    struct State* next;
} State;

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

typedef struct SkipNode {
    char* citizenID;
    Record* record;
    int levels;
    // Array of nexts, one for each level
    struct SkipNode* next[max];
} SkipNode;

typedef struct SkipList {
    SkipNode* head;
    // SkipNode* last;
    int maxLevel;
    char* virus;
    struct SkipList* next;
} SkipList;

#endif