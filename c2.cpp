#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>

template <typename Key, typename Value>
class CuckooHash {
private:
    static constexpr size_t kNumBuckets = 2;
    static constexpr size_t kBucketSize = 4;
    static constexpr size_t kMaxNumKicks = 500;
    std::vector<std::pair<Key, Value>> buckets_[kNumBuckets];
    std::hash<Key> hash_;
    size_t size_;

public:
    CuckooHash() : size_(0) {}

    void Insert(const Key& key, const Value& value) {
        if (Contains(key)) return;
        if (size_ >= kNumBuckets * kBucketSize) {
            std::cerr << "Hash table full!\n";
            return;
        }
        size_t bucket_idx_1 = hash_(key) % kNumBuckets;
        size_t bucket_idx_2 = (bucket_idx_1 ^ hash_(value)) % kNumBuckets;
        size_t num_kicks = 0;
        while (num_kicks < kMaxNumKicks) {
            if (buckets_[bucket_idx_1].size() < kBucketSize) {
                buckets_[bucket_idx_1].emplace_back(key, value);
                ++size_;
                return;
            }
            std::pair<Key, Value> evicted = buckets_[bucket_idx_1].back();
            buckets_[bucket_idx_1].pop_back();
            if (bucket_idx_1 == bucket_idx_2) {
                bucket_idx_2 = (bucket_idx_1 ^ hash_(evicted.first)) % kNumBuckets;
                buckets_[bucket_idx_2].emplace_back(evicted);
            } else if (buckets_[bucket_idx_2].size() < kBucketSize) {
                buckets_[bucket_idx_2].emplace_back(evicted);
            } else {
                std::pair<Key, Value> evicted2 = buckets_[bucket_idx_2].back();
                buckets_[bucket_idx_2].pop_back();
                bucket_idx_1 = (bucket_idx_2 ^ hash_(evicted.first)) % kNumBuckets;
                buckets_[bucket_idx_1].emplace_back(evicted);
                if (bucket_idx_1 == bucket_idx_2) {
                    bucket_idx_2 = (bucket_idx_1 ^ hash_(evicted2.first)) % kNumBuckets;
                    buckets_[bucket_idx_2].emplace_back(evicted2);
                } else if (buckets_[bucket_idx_1].size() < kBucketSize) {
                    buckets_[bucket_idx_1].emplace_back(evicted2);
                } else {
                    bucket_idx_2 = (bucket_idx_1 ^ hash_(evicted2.first)) % kNumBuckets;
                    buckets_[bucket_idx_2].emplace_back(evicted2);
                }
            }
            ++num_kicks;
            bucket_idx_1 = (bucket_idx_2 ^ hash_(evicted.first)) % kNumBuckets;
            bucket_idx_2 = (bucket_idx_1 ^ hash_(evicted.second)) % kNumBuckets;
        }
        std::cerr << "Failed to insert key-value pair!\n";
    }

    void Erase(const Key& key) {
        size_t bucket_idx_1 = hash_(key) % kNumBuckets;
        size_t bucket_idx_2 = (bucket_idx_1 ^ hash_(key)) % kNumBuckets;
        for (size_t i = 0; i < kBucketSize; ++i) {
            if (        buckets_[bucket_idx_1][i].first == key) {
            buckets_[bucket_idx_1].erase(buckets_[bucket_idx_1].begin() + i);
            --size_;
            return;
        }
    }
    for (size_t i = 0; i < kBucketSize; ++i) {
        if (buckets_[bucket_idx_2][i].first == key) {
            buckets_[bucket_idx_2].erase(buckets_[bucket_idx_2].begin() + i);
            --size_;
            return;
        }
    }
}

bool Contains(const Key& key) const {
    size_t bucket_idx_1 = hash_(key) % kNumBuckets;
    size_t bucket_idx_2 = (bucket_idx_1 ^ hash_(key)) % kNumBuckets;
    for (size_t i = 0; i < kBucketSize; ++i) {
        if (buckets_[bucket_idx_1][i].first == key) {
            return true;
        }
    }
    for (size_t i = 0; i < kBucketSize; ++i) {
        if (buckets_[bucket_idx_2][i].first == key) {
            return true;
        }
    }
    return false;
}

Value& operator[](const Key& key) {
    size_t bucket_idx_1 = hash_(key) % kNumBuckets;
    size_t bucket_idx_2 = (bucket_idx_1 ^ hash_(key)) % kNumBuckets;
    for (size_t i = 0; i < kBucketSize; ++i) {
        if (buckets_[bucket_idx_1][i].first == key) {
            return buckets_[bucket_idx_1][i].second;
        }
    }
    for (size_t i = 0; i < kBucketSize; ++i) {
        if (buckets_[bucket_idx_2][i].first == key) {
            return buckets_[bucket_idx_2][i].second;
        }
    }
    Value value{};
    Insert(key, value);
    return operator[](key);
}

size_t size() const { return size_; }
};

// This implementation uses a fixed number of buckets (two in this case) and a fixed bucket size. When inserting a key-value pair, if a bucket is already full, it tries to evict an element and place it in the other bucket. If that bucket is also full, it continues this process, evicting elements until it either succeeds in inserting the new key-value pair or reaches a maximum number of kicks (to prevent infinite loops). When deleting a key-value pair, it checks both buckets for the key and removes it if found. When accessing an element, it checks both buckets for the key and returns the corresponding value if found. If the key is not found, it inserts a default-constructed value and recursively calls the operator[] function to retrieve it.

// Note that this implementation is not thread-safe and does not handle collisions between hash values. If two keys happen to hash to the same value, they will both be placed in the same bucket, potentially causing the insertion process to fail. To handle collisions, you could use a separate chaining strategy where each bucket is a linked list of key-value pairs, or a linear probing strategy where you linearly probe for an empty bucket to insert the key-value pair.
