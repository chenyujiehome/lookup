#include <iostream>
#include <vector>

using namespace std;

class CuckooHash {
private:
    vector<int> table1;
    vector<int> table2;
    int capacity;

    // hash functions
    int hash1(int key) {
        return key % capacity;
    }

    int hash2(int key) {
        return (key / capacity) % capacity;
    }

public:
    CuckooHash(int capacity) {
        this->capacity = capacity;
        table1.assign(capacity, -1);
        table2.assign(capacity, -1);
    }

    // insert a key-value pair into the hash table
    void insert(int key, int value) {
        int i;
        for (i = 0; i < capacity; i++) {
            int index1 = hash1(key);
            int index2 = hash2(key);

            // check if index1 is empty
            if (table1[index1] == -1) {
                table1[index1] = value;
                return;
            }

            // check if index2 is empty
            if (table2[index2] == -1) {
                table2[index2] = value;
                return;
            }

            // swap values and re-hash
            int temp = table1[index1];
            table1[index1] = value;
            value = temp;

            temp = table2[index2];
            table2[index2] = value;
            value = temp;
            key = temp;
        }

        // table is full, rehash
        rehash();
        insert(key, value);
    }

    // search for a key in the hash table
    int search(int key) {
        int index1 = hash1(key);
        int index2 = hash2(key);

        if (table1[index1] == key) {
            return table1[index1];
        }

        if (table2[index2] == key) {
            return table2[index2];
        }

        return -1;
    }

    // rehash the table with new hash functions and double the capacity
    void rehash() {
        capacity *= 2;
        vector<int> oldTable1 = table1;
        vector<int> oldTable2 = table2;
        table1.assign(capacity, -1);
        table2.assign(capacity, -1);

        for (int i = 0; i < oldTable1.size(); i++) {
            if (oldTable1[i] != -1) {
                insert(oldTable1[i], oldTable1[i]);
            }
        }

        for (int i = 0; i < oldTable2.size(); i++) {
            if (oldTable2[i] != -1) {
                insert(oldTable2[i], oldTable2[i]);
            }
        }
    }

    // print the contents of the hash table
    void print() {
        cout << "Table 1: ";
        for (int i = 0; i < table1.size(); i++) {
            cout << table1[i] << " ";
        }
        cout << endl;

        cout << "Table 2: ";
        for (int i = 0; i < table2.size(); i++) {
            cout << table2[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    CuckooHash hashTable(5);
    hashTable.insert(1, 100);
    hashTable.insert(2, 200);
hashTable.insert(3, 300);
hashTable.insert(4, 400);
hashTable.insert(5, 500);
hashTable.print(); // print the contents of the hash table

int value = hashTable.search(1);
if (value != -1) {
    cout << "Key 1 found with value " << value << endl;
} else {
    cout << "Key 1 not found" << endl;
}

value = hashTable.search(6);
if (value != -1) {
    cout << "Key 6 found with value " << value << endl;
} else {
    cout << "Key 6 not found" << endl;
}

return 0;
}
