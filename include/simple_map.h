#pragma once

#include <cstddef>

// Simple map implementation for baremetal environment
template<typename Key, typename Value>
class SimpleMap {
private:
    struct Node {
        Key key;
        Value value;
        Node* next;
        
        Node(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
    };
    
    Node* head;
    size_t size_;
    
public:
    SimpleMap() : head(nullptr), size_(0) {}
    
    ~SimpleMap() {
        clear();
    }
    
    // Copy constructor
    SimpleMap(const SimpleMap& other) : head(nullptr), size_(0) {
        Node* current = other.head;
        while (current) {
            insert(current->key, current->value);
            current = current->next;
        }
    }
    
    // Assignment operator
    SimpleMap& operator=(const SimpleMap& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current) {
                insert(current->key, current->value);
                current = current->next;
            }
        }
        return *this;
    }
    
    void insert(const Key& key, const Value& value) {
        // Check if key already exists
        Node* current = head;
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        // Create new node
        Node* new_node = new Node(key, value);
        new_node->next = head;
        head = new_node;
        size_++;
    }
    
    Value* find(const Key& key) {
        Node* current = head;
        while (current) {
            if (current->key == key) {
                return &current->value;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    // Operator[] for map[key] = value syntax
    Value& operator[](const Key& key) {
        // Check if key already exists
        Node* current = head;
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        
        // Key doesn't exist, create new node with default value
        Node* new_node = new Node(key, Value{});
        new_node->next = head;
        head = new_node;
        size_++;
        return new_node->value;
    }
    
    bool erase(const Key& key) {
        if (!head) return false;
        
        if (head->key == key) {
            Node* to_delete = head;
            head = head->next;
            delete to_delete;
            size_--;
            return true;
        }
        
        Node* current = head;
        while (current->next) {
            if (current->next->key == key) {
                Node* to_delete = current->next;
                current->next = current->next->next;
                delete to_delete;
                size_--;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    void clear() {
        while (head) {
            Node* to_delete = head;
            head = head->next;
            delete to_delete;
        }
        size_ = 0;
    }
    
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    
    // Iterator-like access
    class Iterator {
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
        
        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }
        
        Node& operator*() { return *current; }
    };
    
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};
