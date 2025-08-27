#pragma once
#include <atomic>
#include <vector>
#include <optional>
#include <functional>
#include <memory>
#include <functional>
#include <mutex>

template<typename K, typename V>
class LockFreeHashMap {
public:
    explicit LockFreeHashMap(size_t capacity = 1024)
        : buckets_(capacity), size_(0) {
        for (auto& bucket : buckets_) {
            bucket.head.store(nullptr, std::memory_order_relaxed);
        }
    }

    ~LockFreeHashMap() {
        for (auto& bucket : buckets_) {
            Node* node = bucket.head.load();
            while (node) {
                Node* next = node->next.load();
                delete node;
                node = next;
            }
        }
    }

    bool insert(const K& key, const V& value) {
        size_t index = hash(key);
        Node* newNode = new Node(key, value);

        Node* currentHead = buckets_[index].head.load(std::memory_order_acquire);
        newNode->next.store(currentHead, std::memory_order_release);

        while (!buckets_[index].head.compare_exchange_weak(
            currentHead, newNode,
            std::memory_order_release,
            std::memory_order_relaxed)) {

            newNode->next.store(currentHead, std::memory_order_release);
        }

        size_.fetch_add(1, std::memory_order_relaxed);
        return true;
    }

    std::optional<V> find(const K& key) const {
        size_t index = hash(key);
        Node* node = buckets_[index].head.load(std::memory_order_acquire);

        while (node) {
            if (node->key == key) {
                return node->value;
            }
            node = node->next.load(std::memory_order_acquire);
        }
        return std::nullopt;
    }

    bool remove(const K& key) {
        size_t index = hash(key);
        // This remove is not fully lock-free (simple traversal + pointer updates)
        // but it's fine for the prototype.
        Node* node = buckets_[index].head.load(std::memory_order_acquire);
        Node* prev = nullptr;

        while (node) {
            if (node->key == key) {
                if (prev) {
                    prev->next.store(node->next.load(std::memory_order_acquire),
                                    std::memory_order_release);
                } else {
                    buckets_[index].head.store(node->next.load(std::memory_order_acquire),
                                             std::memory_order_release);
                }

                delete node;
                size_.fetch_sub(1, std::memory_order_relaxed);
                return true;
            }

            prev = node;
            node = node->next.load(std::memory_order_acquire);
        }
        return false;
    }

    size_t size() const {
        return size_.load(std::memory_order_relaxed);
    }

    // Iterate all key/value pairs. This is not wait-free: it takes a snapshot by walking buckets.
    // Use only for debugging / SELECT in prototype.
    void for_each(const std::function<void(const K&, const V&)>& fn) const {
        for (const auto &bucket : buckets_) {
            Node* node = bucket.head.load(std::memory_order_acquire);
            while (node) {
                fn(node->key, node->value);
                node = node->next.load(std::memory_order_acquire);
            }
        }
    }

private:
    struct Node {
        K key;
        V value;
        std::atomic<Node*> next;

        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    struct Bucket {
        std::atomic<Node*> head;
    };

    std::vector<Bucket> buckets_;
    std::atomic<size_t> size_;

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % buckets_.size();
    }
};
