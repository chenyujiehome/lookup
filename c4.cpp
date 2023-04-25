#include <iostream>
#include <vector>
#include <algorithm>

//定义哈希表的大小
#define MAXN 100000

using namespace std;

//定义哈希表的桶的大小
#define BUCKET_SIZE 4

//定义哈希函数的个数
#define HASH_FUNC_NUM 2

//定义哈希函数1
unsigned int hash_func1(unsigned int key) {
    return key % MAXN;
}

//定义哈希函数2
unsigned int hash_func2(unsigned int key) {
    return (key / MAXN) % MAXN;
}

//定义哈希节点
struct hash_node {
    unsigned int key;
    unsigned int value;
};

//定义哈希表
class cuckoo_hash {
private:
    vector<hash_node> buckets[BUCKET_SIZE];
    vector<unsigned int> (*hash_func[HASH_FUNC_NUM])(unsigned int);
public:
    cuckoo_hash() {
        //初始化哈希函数
        hash_func[0] = &hash_func1;
        hash_func[1] = &hash_func2;
    }
    //插入键值对
    void insert(unsigned int key, unsigned int value) {
        //根据哈希函数1计算桶的下标
        unsigned int bucket_index = (*hash_func[0])(key) % BUCKET_SIZE;
        //查找桶中是否已经存在该键
        vector<hash_node>& bucket = buckets[bucket_index];
        for (int i = 0; i < bucket.size(); ++i) {
            if (bucket[i].key == key) {
                bucket[i].value = value;
                return;
            }
        }
        //插入新的键值对到桶中
        hash_node new_node = {key, value};
        bucket.push_back(new_node);
        //如果桶已经满了，则进行替换操作
        if (bucket.size() == BUCKET_SIZE) {
            replace(bucket_index, 0);
        }
    }
    //查找键的值
    unsigned int search(unsigned int key) {
        //根据哈希函数1计算桶的下标
        unsigned int bucket_index = (*hash_func[0])(key) % BUCKET_SIZE;
        //在桶中查找键的值
        vector<hash_node>& bucket = buckets[bucket_index];
        for (int i = 0; i < bucket.size(); ++i) {
            if (bucket[i].key == key) {
                return bucket[i].value;
            }
        }
        //如果第一个桶中没有找到，则根据哈希函数2计算桶的下标
        bucket_index = (*hash_func[1])(key) % BUCKET_SIZE;
        //在第二个桶中查找键的值
        bucket = buckets[bucket_index];
        for (int i = 0; i < bucket.size(); ++i) {
            if (bucket[i].key == key) {
                return bucket[i].value;
            }
        }
        //如果两个桶中都没有找到，则返回0
        return 0;
    }
    //替换桶中的键值对
    void replace(unsigned int bucket_index, int depth) {
//如果替换的深度太深，则认为哈希表已经满了
if (depth > BUCKET_SIZE * HASH_FUNC_NUM) {
throw "Hash table is full.";
}
//取出要替换的键值对
vector<hash_node>& bucket = buckets[bucket_index];
hash_node replace_node = bucket[depth % BUCKET_SIZE];
//根据哈希函数1计算桶的下标
unsigned int next_bucket_index = (*hash_func[0])(replace_node.key) % BUCKET_SIZE;
//如果要替换的桶不是当前桶，则将键值对插入到要替换的桶中
if (next_bucket_index != bucket_index) {
//查找要替换的桶中是否已经存在该键
vector<hash_node>& next_bucket = buckets[next_bucket_index];
for (int i = 0; i < next_bucket.size(); ++i) {
if (next_bucket[i].key == replace_node.key) {
next_bucket[i].value = replace_node.value;
//递归调用替换函数，继续替换要替换的桶中的键值对
replace(next_bucket_index, depth + 1);
return;
}
}
//插入新的键值对到要替换的桶中
hash_node new_node = {replace_node.key, replace_node.value};
next_bucket.push_back(new_node);
//如果要替换的桶已经满了，则递归调用替换函数，继续替换要替换的桶中的键值对
if (next_bucket.size() == BUCKET_SIZE) {
replace(next_bucket_index, depth + 1);
}
}
//否则，将键值对插入到当前桶的下一个位置中
else {
int next_index = (depth + 1) % BUCKET_SIZE;
bucket[next_index] = replace_node;
//递归调用替换函数，继续替换当前桶中的键值对
replace(bucket_index, depth + 1);
}
}
};

int main() {
cuckoo_hash hash_table;
//插入测试数据
hash_table.insert(1, 10);
hash_table.insert(2, 20);
hash_table.insert(3, 30);
hash_table.insert(4, 40);
hash_table.insert(5, 50);
//查找测试数据
cout << hash_table.search(1) << endl;
cout << hash_table.search(2) << endl;
cout << hash_table.search(3) << endl;
cout << hash_table.search(4) << endl;
cout << hash_table.search(5) << endl;
return 0;
}


// 上述代码中，使用两个哈希函数对键进行散列，将键值对存储到不同的桶中。当桶中已经存在该键时，更新其值；当桶已经满了时，将其替换到另一个桶中，如果要替换的桶也已经满了，则继续进行递归替换操作。
