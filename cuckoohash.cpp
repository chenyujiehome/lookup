#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_ITERATIONS 5000
#define HASH_1(key, size) (key % size)
#define HASH_2(key, size) ((key / size) % size)

typedef struct {
    unsigned int *keys;
    unsigned int *values;
    int size;
} HashTable;

unsigned int hash(unsigned int key, int size, int index) {
    return (HASH_1(key, size) + index * HASH_2(key, size)) % size;
}

HashTable *createHashTable(int size) {
    HashTable *table = (HashTable*) malloc(sizeof(HashTable));
    table->keys = (unsigned int*) calloc(size, sizeof(unsigned int));
    table->values = (unsigned int*) calloc(size, sizeof(unsigned int));
    table->size = size;
    return table;
}

void destroyHashTable(HashTable *table) {
    free(table->keys);
    free(table->values);
    free(table);
}

int insert(HashTable *table, unsigned int key, unsigned int value) {
    int index1 = hash(key, table->size, 0);
    int index2 = hash(key, table->size, 1);
    int i;

    for (i = 0; i < MAX_ITERATIONS; i++) {
        if (table->keys[index1] == 0) {
            table->keys[index1] = key;
            table->values[index1] = value;
            return 1;
        }
        if (table->keys[index2] == 0) {
            table->keys[index2] = key;
            table->values[index2] = value;
            return 1;
        }
        unsigned int tmp_key = table->keys[index1];
        unsigned int tmp_value = table->values[index1];
        table->keys[index1] = key;
        table->values[index1] = value;
        key = tmp_key;
        value = tmp_value;
        index1 = hash(key, table->size, i+1);
        index2 = hash(key, table->size, i+2);
    }
    return 0;
}

int search(HashTable *table, unsigned int key, unsigned int *value) {
    int index1 = hash(key, table->size, 0);
    int index2 = hash(key, table->size, 1);
    int i;

    for (i = 0; i < MAX_ITERATIONS; i++) {
        if (table->keys[index1] == key) {
            *value = table->values[index1];
            return 1;
        }
        if (table->keys[index2] == key) {
            *value = table->values[index2];
            return 1;
        }
        index1 = hash(key, table->size, i+1);
        index2 = hash(key, table->size, i+2);
    }
    return 0;
}

int main() {
    HashTable *table = createHashTable(10);
    insert(table, 5, 10);
    insert(table, 15, 20);
    insert(table, 25, 30);

    unsigned int value;
    if (search(table, 15, &value)) {
        printf("Value for key 15: %d\n", value);
    }
    if (search(table, 25, &value)) {
        printf("Value for key 25: %d\n", value);
    }

    destroyHashTable(table);
    return 0;}
