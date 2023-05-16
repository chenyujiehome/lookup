#include <cstdlib>
#include <iostream>
#include <vector>

#define MAX_ITERATIONS 5000

typedef struct {
    std::vector<unsigned int> keys[2];
    std::vector<unsigned int> values[2];
    int size;
} CuckooHashTable;

unsigned int hash1(unsigned int key, int size) {
    return key % size;
}

unsigned int hash2(unsigned int key, int size) {
    return (key / size) % size;
}

CuckooHashTable *createHashTable(int size) {
    CuckooHashTable *table = new CuckooHashTable;
    table->keys[0].resize(size, 0);
    table->keys[1].resize(size, 0);
    table->values[0].resize(size, 0);
    table->values[1].resize(size, 0);
    table->size = size;
    return table;
}

void destroyHashTable(CuckooHashTable *table) {
    delete table;
}

bool insert(CuckooHashTable *table, unsigned int key, unsigned int value) {
    unsigned int index;
    unsigned int curr_key = key;
    unsigned int curr_value = value;
    int table_id = 0;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        index = table_id ? hash2(curr_key, table->size) : hash1(curr_key, table->size);
        if (table->keys[table_id][index] == 0) {
            table->keys[table_id][index] = curr_key;
            table->values[table_id][index] = curr_value;
            return true;
        }
        std::swap(table->keys[table_id][index], curr_key);
        std::swap(table->values[table_id][index], curr_value);
        table_id = 1 - table_id;
    }
    return false;
}

bool search(CuckooHashTable *table, unsigned int key, unsigned int *value) {
    int index1 = hash1(key, table->size);
    int index2 = hash2(key, table->size);

    if (table->keys[0][index1] == key) {
        *value = table->values[0][index1];
        return true;
    }
    if (table->keys[1][index2] == key) {
        *value = table->values[1][index2];
        return true;
    }
    return false;
}

int main() {
    CuckooHashTable *table = createHashTable(10);
    insert(table, 5, 10);
    insert(table, 15, 20);
    insert(table, 25, 30);

    unsigned int value;
    if (search(table, 15, &value)) {
        std::cout << "Value for key 15: " << value << std::endl;
    }
    if (search(table, 25, &value)) {
        std::cout << "Value for key 25: " << value << std::endl;
    }

    destroyHashTable(table);
    return 0;
}
//两个表改为一个表
//8个hash函数
//不同字节的插入
//插入函数核对