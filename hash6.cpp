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
    table->data = new uint[size];  // allocate memory for the maximum possible size of keys and values
    memset(table->data, 0, size*sizeof(uint));  // initialize to 0
    table->size = size;
    return table;
}

void destroyHashTable(CuckooHashTable *table) {
    delete[] table->data;  // free the allocated memory
    delete table;
}

void insertKV(uint key, vector<uint>& value, uint* data){
    sizeKey sk;
    sk.key = key;
    sk.valueSize = value.size();
    data[0] = *(uint*)&sk;  // store the size and key
    memcpy(data+1, value.data(), value.size()*sizeof(uint));  // copy the values
}

uint calcFlowSize(uint valueSize) {
    // return valuesize+1
    return valueSize + 1;
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
            uint index = hashFunc(curr_key, table->size, j, flowsize) ;

            if (table->data[index] == 0) {  // if empty slot found
                insertKV(curr_key, curr_value, table->data+index);
                return true;
            }

            sizeKey* sk = (sizeKey*)&table->data[index];
            if (sk->key == curr_key) {  // if key already exists
                insertKV(curr_key, curr_value, table->data+index);
                return true;
            }

            if (calcFlowSize(sk->valueSize) <= flowsize) {  // if existing value is smaller or equal in size
                uint temp_key = curr_key;
                vector<uint> temp_value = curr_value;
                curr_key = sk->key;
                curr_value = vector<uint>(table->data+index+1, table->data+index+1+sk->valueSize);
                insertKV(temp_key, temp_value, table->data+index);
            }
        }
    }

    return false;
}

bool search(CuckooHashTable *table, uint key, vector<uint>& value) {
    value.clear();

    for (int i = 0; i < HASH_FUNC_COUNT; i++) {
        for(int j=2;j<=16;j=j*2){
        uint index = hashFunc(key, table->size, i,j );  // minimum flow size is 2
        sizeKey* sk = (sizeKey*)&table->data[index];

        if (sk->key == key) {
            value = vector<uint>(table->data+index+1, table->data+index+1+sk->valueSize);
            return true;
        }}
    }

    return false;
}

double loadFactor(CuckooHashTable *table) {
    int count = 0;
    for (int i = 0; i < table->size; i+=1) {
        if (table->data[i] != 0) {
            count++;
        }
    }
    return (double)count / table->size;
}

#include <random>

int main() {
    CuckooHashTable *table = createHashTable(1000000);
    random_device rd;
    mt19937 gen(rd());  //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<uint> dis(1, 1000000);  // to generate random keys

    int n_values = 6666;
    for (uint valueSize = 1; valueSize <= 15; ++valueSize) {
        for (int i = 0; i < n_values; ++i) {
            uint key = dis(gen);
            vector<uint> value(valueSize, dis(gen));
            insert(table, key, value);
        }
 
    }
       cout << "Load factor after inserting :"  << loadFactor(table) << endl;
    destroyHashTable(table);
    return 0;
}
