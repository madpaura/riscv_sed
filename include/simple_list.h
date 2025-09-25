#pragma once

#include <cstddef>

// Simple doubly-linked list implementation for baremetal environment
template<typename T>
class SimpleList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        
        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
        
        template<typename... Args>
        Node(Args&&... args) : data(args...), next(nullptr), prev(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    size_t size_;
    
public:
    SimpleList() : head(nullptr), tail(nullptr), size_(0) {}
    
    ~SimpleList() {
        clear();
    }
    
    // Copy constructor
    SimpleList(const SimpleList& other) : head(nullptr), tail(nullptr), size_(0) {
        Node* current = other.head;
        while (current) {
            push_back(current->data);
            current = current->next;
        }
    }
    
    // Assignment operator
    SimpleList& operator=(const SimpleList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }
    
    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        size_++;
    }
    
    void push_front(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        size_++;
    }
    
    template<typename... Args>
    void emplace_back(Args&&... args) {
        Node* new_node = new Node(args...);
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        size_++;
    }
    
    template<typename... Args>
    void emplace_front(Args&&... args) {
        Node* new_node = new Node(args...);
        if (!head) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        size_++;
    }
    
    void pop_back() {
        if (!tail) return;
        
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node* to_delete = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete to_delete;
        }
        size_--;
    }
    
    void pop_front() {
        if (!head) return;
        
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node* to_delete = head;
            head = head->next;
            head->prev = nullptr;
            delete to_delete;
        }
        size_--;
    }
    
    T& front() { return head->data; }
    const T& front() const { return head->data; }
    
    T& back() { return tail->data; }
    const T& back() const { return tail->data; }
    
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    
    void clear() {
        while (head) {
            Node* to_delete = head;
            head = head->next;
            delete to_delete;
        }
        tail = nullptr;
        size_ = 0;
    }
    
    // Iterator class
    class Iterator {
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
        
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }
        
        Iterator& operator--() {
            if (current) current = current->prev;
            return *this;
        }
        
        T& operator*() { return current->data; }
        const T& operator*() const { return current->data; }
        
        T* operator->() { return &current->data; }
        const T* operator->() const { return &current->data; }
    };
    
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
    
    const Iterator begin() const { return Iterator(head); }
    const Iterator end() const { return Iterator(nullptr); }
};
