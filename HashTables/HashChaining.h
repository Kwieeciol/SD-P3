#pragma once

struct Node {
    int   key;
    int   value;
    Node* next;

    Node(int k, int v);
};

class HashTableChaining {
private:
    static const int DEFAULT_CAPACITY = 16;

    Node** buckets;
    int    capacity;
    int    size;

    int hash(int key) const;

public:
    explicit HashTableChaining(int cap = DEFAULT_CAPACITY);
    ~HashTableChaining();

    void insert(int key, int value);
    bool remove(int key);
    int* search(int key) const;

    void clear();

    int getSize()     const;
    int getCapacity() const;

    void print() const;
};