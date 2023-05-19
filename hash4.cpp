#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include<bits/stdc++.h>
#define MAX_ITERATIONS 8
#define HASH_FUNC_COUNT 8
typedef unsigned int uint;
using namespace std;

struct sizeKey {
    uint valueSize:4;
    uint key:28;
};

struct CuckooHashTable {
    uint* data;
    uint size;
};

uint roundUpToNextPowerOfTwo(uint v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

uint hashFunc(uint key, uint size, uint hashIndex,uint flowsize) {
    int up = roundUpToNextPowerOfTwo(flowsize);
    int nodeNum = size/up;
    switch (hashIndex) {
    case 0:
        return (key * 23) % nodeNum*up;
    case 1:
        return (key * 19) % nodeNum*up;
    case 2:
        return (key * 3) % nodeNum*up;
    case 3:
        return (key * 5) % nodeNum*up;
    case 4:
        return (key * 7) % nodeNum*up;
    case 5:
        return (key * 11) % nodeNum*up;
    case 6:
        return (key * 13) % nodeNum*up;
    case 7:
        return (key * 17) % nodeNum*up;
    default:
        return key % nodeNum*up;
    }
}

CuckooHashTable *createHashTable(uint size) {
    CuckooHashTable *table = new CuckooHashTable;
    table->data = new uint[size*16];  // allocate memory for the maximum possible size of keys and values
    memset(table->data, 0, size*16*sizeof(uint));  // initialize to 0
    table->size = size;
    return table;
}

void destroyHashTable(CuckooHashTable *table) {
    delete[] table->data;  // free the allocated memory
    delete table;
}

void preprocess(uint key, vector<uint>& value, uint* data){
    sizeKey sk;
    sk.key = key;
    sk.valueSize = value.size();
    data[0] = *(uint*)&sk;  // store the size and key
    memcpy(data+1, value.data(), value.size()*sizeof(uint));  // copy the values
}

uint calcFlowSize(uint valueSize) {
    // round up to the next power of 2
    return pow(2, ceil(log(valueSize+1)/log(2)));
}

bool insert(CuckooHashTable *table, uint key, vector<uint> value) {
    if (value.size() == 0 || key == 0 || (find(value.begin(), value.end(), 0) != value.end())) {
        return false;
    }

    uint flowsize = calcFlowSize(value.size());
    uint curr_key = key;
    vector<uint> curr_value = value;

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        for (int j = 0; j < HASH_FUNC_COUNT; j++) {
            uint index = hashFunc(curr_key, table->size, j, flowsize) * 16;

            if (table->data[index] == 0) {  // if empty slot found
                preprocess(curr_key, curr_value, table->data+index);
                return true;
            }

            sizeKey* sk = (sizeKey*)&table->data[index];
            if (sk->key == curr_key) {  // if key already exists
                preprocess(curr_key, curr_value, table->data+index);
                return true;
            }

            if (calcFlowSize(sk->valueSize) <= flowsize) {  // if existing value is smaller or equal in size
                uint temp_key = curr_key;
                vector<uint> temp_value = curr_value;
                curr_key = sk->key;
                curr_value = vector<uint>(table->data+index+1, table->data+index+1+sk->valueSize);
                preprocess(temp_key, temp_value, table->data+index);
            }
        }
    }

    return false;
}

bool search(CuckooHashTable *table, uint key, vector<uint>& value) {
    value.clear();

    for (int i = 0; i < HASH_FUNC_COUNT; i++) {
        uint index = hashFunc(key, table->size, i, calcFlowSize(1)) * 16;  // minimum flow size is 2
        sizeKey* sk = (sizeKey*)&table->data[index];

        if (sk->key == key) {
            value = vector<uint>(table->data+index+1, table->data+index+1+sk->valueSize);
            return true;
        }
    }

    return false;
}

double loadFactor(CuckooHashTable *table) {
    int count = 0;
    for (int i = 0; i < table->size*16; i+=16) {
        if (table->data[i] != 0) {
            count++;
        }
    }
    return (double)count / table->size;
}

int main() {
    CuckooHashTable *table = createHashTable(10);
    vector<uint> value = {10};
    insert(table, 5, value);
    value = {20};
    insert(table, 15, value);
    value = {30};
    insert(table, 25, value);

    vector<uint> result;
    if (search(table, 15, result)) {
        cout << "Value for key 15: " << result[0] << endl;
    }
    if (search(table, 25, result)) {
        cout << "Value for key 25: " << result[0] << endl;
    }
    cout << "Load factor: " << loadFactor(table) << endl;
    destroyHashTable(table);
    return 0;
}
