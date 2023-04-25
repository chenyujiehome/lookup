#include <iostream>
#include <vector>
#include <stdexcept>

const int MAX_ITERATIONS = 100;

class CuckooHashTable {
public:
    CuckooHashTable(int size)
        : table1(size, -1), table2(size, -1), size(size) {}

    void insert(int key, int value) {
        int position1 = hash1(key);
        int position2 = hash2(key);

        for (int i = 0; i < MAX_ITERATIONS; ++i) {
            if (table1[position1] == -1) {
                table1[position1] = value;
                return;
            }

            if (table2[position2] == -1) {
                table2[position2] = value;
                return;
            }

            std::swap(value, table1[position1]);
            std::swap(value, table2[position2]);
        }

        rehash();
        insert(key, value);
    }

    int search(int key) {
        int position1 = hash1(key);
        int position2 = hash2(key);

        if (table1[position1] != -1) {
            return table1[position1];
        }

        if (table2[position2] != -1) {
            return table2[position2];
        }

        throw std::runtime_error("Key not found");
    }

    void remove(int key) {
        int position1 = hash1(key);
        int position2 = hash2(key);

        if (table1[position1] != -1) {
table1[position1] = -1;
return;
}
    if (table2[position2] != -1) {
        table2[position2] = -1;
        return;
    }

    throw std::runtime_error("Key not found");
}
private:
int hash1(int key) {
return key % size;
}
int hash2(int key) {
    return (key / size) % size;
}

void rehash() {
    std::vector<int> old_table1(table1);
    std::vector<int> old_table2(table2);

    table1.assign(size, -1);
    table2.assign(size, -1);

    for (int value : old_table1) {
        if (value != -1) {
            insert(value, value);
        }
    }

    for (int value : old_table2) {
        if (value != -1) {
            insert(value, value);
        }
    }
}

std::vector<int> table1;
std::vector<int> table2;
int size;
};

int main() {
CuckooHashTable table(10);
table.insert(1, 1);
table.insert(2, 2);
table.insert(3, 3);
table.insert(11, 11);

std::cout << "Search for key 1: " << table.search(1) << std::endl;
std::cout << "Search for key 11: " << table.search(11) << std::endl;

table.remove(1);

try {
    std::cout << "Search for key 1: " << table.search(1)<< std::endl;
} catch (const std::runtime_error& e) {
std::cout << "Error: " << e.what() << std::endl;
}
return 0;
}

// This code creates a Cuckoo hash table and demonstrates how to insert, search, and remove keys. In the `main()` function, it inserts a few keys, searches for them, removes one key, and then tries to search for the removed key, which results in an exception being thrown.
