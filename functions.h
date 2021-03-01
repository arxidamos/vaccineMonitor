#ifndef __FUNCTIONS__
#define __FUNCTIONS__ 
#include "structs.h"

// functions.c 
// Linked list functions
Record* createRecord (char* citizenID, char* fName, char* lName, char* country, int age, char* virus,  Date vaccDate);
Record* insertSortedRecord (Record* head, char* citizenID, char* fName, char* lName, char* country, int age, char* virus, Date vaccDate);
int checkDuplicate (Record* head, char* citizenID, char* fName, char* lName, char* country, int age, char* virus, Date vaccDate);
void printRecordsList (Record* record);
void freeRecordList (Record* head);
// treeFunctions.c
// TreeNode* createTreeNode (Patient* patient, Date entryDate);
// TreeRoot* createTreeRoot (char* keyID, Patient* patient, Date entryDate);
// TreeNode* insertTreeNode (TreeNode* root, Patient* patient);
// void freeTree (TreeRoot* tree);
// void freeTreeNode (TreeNode* node);
// int height (TreeNode *root);
// int getBalance (TreeNode *root);
// int max(int x, int y) ;
// TreeNode* latestNode (TreeNode* root);
// TreeNode* oldestNode (TreeNode* root); 
// TreeNode* rotateLeft (TreeNode* root);
// TreeNode* rotateRight (TreeNode* root);
// TreeNode* rebalance (TreeNode* root);
// void printTreeAsList (TreeNode *root);
// void printTree (TreeRoot* tree);
// void viewTree (TreeNode* root, int depth);

// // mainFunctions.c
// // Command functions
// void globalDiseaseStats(HashTableBucket** ht, int ht_size, Date date1, Date date2);
// void diseaseFrequency (HashTableBucket** diseaseHT, int disease_ht_size, char* diseaseID, char* country, Date date1, Date date2);
// void topkDiseases (HashTableBucket** countryHT, int country_ht_size, char* country, int k, Date date1, Date date2);
// void topkCountries (HashTableBucket** diseaseHT, int disease_ht_size, char* diseaseID, int k, Date date1, Date date2);
// void recordPatientExit (Patient* head, char* recordID, Date newDate);
// void numCurrentPatients (HashTableBucket** diseaseHT, int disease_ht_size, char* diseaseID);
// void numCurrentPatientsAll (HashTableBucket** diseaseHT, int disease_ht_size);
// // Helper functions
// int countTreeNodes (TreeNode* root, Date date1, Date date2, int counter);
// int countDiseaseFrequency (TreeNode* root, Date date1, Date date2, int counter, char* country);
// int countDiseaseFrequencyAll (TreeNode* root, Date date1, Date date2, int counter);
// HeapTree* countTopkDiseases (HeapTree* heap, TreeNode* root, Date date1, Date date2);
// HeapTree* countTopkCountries (HeapTree* heap, TreeNode* root, Date date1, Date date2);
// int countCurrentPatients (TreeNode* root, int counter);
// Date getTime ();


#endif