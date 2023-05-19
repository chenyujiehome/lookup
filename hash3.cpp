#include <cstdlib>
#include <iostream>
#include <vector>

#define MAX_ITERATIONS 8
#define HASH_FUNC_COUNT 8
#define unsigned int uint
using namespace std;

typedef struct {


      uint valueSize:4;
      uint key:28;
} sizeKey;
typedef struct {
   vector<uint> keyValues;
    uint size;
} CuckooHashTable;

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
    table->keyValues.resize(size, 0);
    table->size = size;
    return table;
}

void destroyHashTable(CuckooHashTable *table) {
    delete table;
}
void preprocess(const uint& key, vector<uint>& value){//将value的size和key存入value的第一个元素中   
sizeKey keyV;
keyV.key=key;
keyV.valueSize=value.size();
value.insert(value.begin(),*((uint *)&keyV));
return;
}
bool (CuckooHashTable *table,uint index,uint flowsize){
if (table.keyValues[index]!=0){
uint tsize=(((sizeKey)(table.keyValues[index])).valueSize)+1;

if(tsize>flowsize){
//insert next;
}
else if(tsize==flowsize){
//swap and insert ;
}
else{
//查询下列所有
}
}

}
bool insert(CuckooHashTable *table, uint key, vector<uint> value) {

    if (value.size() == 0||key==0||(find(value.begin(), value.end(),0)!=value.end())){
        return false;
    }
       preprocess( key,  value);
    uint valueSize = value.size();
    uint flowsize = valueSize+1;
    uint index;
    uint curr_key = key;
    vector<uint> curr_value = value;
//查询是大插小还是小插大，小插大小顺序hash，大插小小被踢出小顺序hash，判断大小的逻辑
//添加flowsize属性在cuckoohashtable中，插入问题，查询问题，交换逻辑都要改
//flowsize 2,4,8,16
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        for (int j = 0; j < HASH_FUNC_COUNT; j++) {
            index = hashFunc(curr_key, table->size, j,flowsize);
            if (table->keyValues[index] == 0) {
                table->keyValues[index] = curr_key;
                table->values[index] = curr_value;
                return true;
            }
            if (table->keys[index] == curr_key) {
                table->values[index] = curr_value;
                return true;
            }
        }
        int evictionIndex = rand() % HASH_FUNC_COUNT;
        //随机rand改为顺序读取hash函数(开始数随机数，后面顺序读取)
    
        index = hashFunc(curr_key, table->size, evictionIndex,flowsize);
        swap(table->keys[index], curr_key);
        swap(table->values[index], curr_value);
    }
    return false;
}

bool search(CuckooHashTable *table, uint key, uint *value) {
    for (int i = 0; i < HASH_FUNC_COUNT; i++) {
        int index = hashFunc(key, table->size, i);
        if (table->keys[index] == key) {
            *value = table->values[index];
            return true;
        }
    }
    return false;
}
//输出cuckoohashtable的装载率
double loadFactor(CuckooHashTable *table) {
    int count = 0;
    for (int i = 0; i < table->size; i++) {
        if (table->keys[i] != 0) {
            count++;
        }
    }
    return (double)count / table->size;
}

int main() {
    CuckooHashTable *table = createHashTable(10);
    insert(table, 5, 10);
    insert(table, 15, 20);
    insert(table, 25, 30);

    uint value;
    if (search(table, 15, &value)) {
        cout << "Value for key 15: " << value << endl;
    }
    if (search(table, 25, &value)) {
        cout << "Value for key 25: " << value << endl;
    }
//输出cuckoohash的装载率
    cout << "Load factor: " << loadFactor(table) << endl;
    destroyHashTable(table);
    return 0;
}
//每个key value对在table中所占的空间是不同的，
//flowsize :2-


// uint function2(uint num)//经典解法
// {
//       uint count=0;
//      while(num)
//       {
//            num=num&(num-1);
//            count++;
//      }
// return count;
// }
