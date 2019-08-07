#include <stdio.h>
#include <stdlib.h>

struct  entry_s{
  int *key;
  int val;
  struct entry_s *next;
};
typedef struct entry_s entry_t;

typedef struct {
  int size;
  struct entry_s **table;
} arrayHashTable;

/* Initilaize an enstry struck and returns a pointer to it */
entry_t* createEntry(int *key, int val){
  entry_t *e;
  e = (entry_t*) malloc(sizeof(entry_t));
  e->key = key;
  e->val = val;
  e->next = NULL;
  return e;
}

/*Initilaize array hash table, 
param: size, the size of the array which stores the entrys.
returns pointer to the object*/
arrayHashTable* createArrayHashTable(int size){
  arrayHashTable *table;
  table = (arrayHashTable*) malloc(sizeof(arrayHashTable));
  table->size = size;
  table->table = (entry_t**) malloc(sizeof(entry_t*) * table->size);
  for (int i = 0; i < size; i++){
    table->table[i] = NULL;
  }
  return table;
}

/*Hash function returns an int based on the forst element in the key array, the length of the array on the size of the hashTable */
int hashFunction(arrayHashTable *table, int *key, int keySize){
  int sum = 0;
  for (int i = 0; i < keySize; i++){
    sum = sum + key[i];
  }
  int index = sum % table->size;
  return index;
}

/*Insert a key and value parin into the hashtable
Params:
key, an array in ints
keySize, the length of the key array
val, the value to bind to the key*/
void hashInsert(arrayHashTable *table, int *key, int keySize, int val){
  int index = hashFunction(table, key, keySize);
  entry_t *newEntry = createEntry(key, val);
  if(table->table[index] == NULL){
    table->table[index] = newEntry;
  }
  else { // Chaining:
    entry_t *nextEntry = table->table[index];
    while(nextEntry->next != NULL){
      nextEntry = nextEntry->next;
    }
    nextEntry->next = newEntry;
  }
  
}

/*hashGet returns the value of the passed key*/
int hashGet(arrayHashTable *table, int *key, int keySize){
  int index = hashFunction(table, key, keySize);
  entry_t *e = table->table[index];
  if (e == NULL) return -1; // No entry at the index
  while (e->key != key && e->next != NULL){ // Handles chaining
    e = e->next;
  }
  if (e->key != key) {
    return -1;
  }
  return e->val;
}

/*Print alle the values contained in the hash table*/
void printHashTable(arrayHashTable *table){
  for(int i = 0; i < table->size; i++){
    entry_t *e = table->table[i];
    while(e != NULL){
      printf("%d, ", e->val);
      e = e->next;
    }
  }
}

// Test Code:
// int main(){
//   int size = 6;
//   int hashSize = (size*size);
//   arrayHashTable *table = createArrayHashTable(hashSize);
// 
//   int arr1[] = {9, 3, 8, 5, 7, 13};
//   int arr2[] = {9, 3, 8, 5};
//   int arr3[] = {9, 3, 8, 5, 0};
//   printf("Index %d\n", hashFunction(table, arr2, sizeof(arr2)/sizeof(int)));
//   printf("Index %d\n", hashFunction(table, arr3, sizeof(arr3)/sizeof(int)));
//   hashInsert(table, arr1, sizeof(arr1)/sizeof(int), 1);
//   hashInsert(table, arr2, sizeof(arr2)/sizeof(int), 2);
//   hashInsert(table, arr3, sizeof(arr3)/sizeof(int), 400);
// 
//   printf("%d\n", hashGet(table, arr1, sizeof(arr1)/sizeof(int)));
//   printf("%d\n", hashGet(table, arr2, sizeof(arr2)/sizeof(int)));
//   printf("%d\n", hashGet(table, arr3, sizeof(arr3)/sizeof(int)));
//   printHashTable(table);
//   return 0;
// }









