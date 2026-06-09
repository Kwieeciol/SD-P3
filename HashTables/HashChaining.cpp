#include "HashChaining.h"
#include <iostream>

Node::Node(int k, int v) : key(k), value(v), next(nullptr) {}


HashTableChaining::HashTableChaining(int cap)
    : capacity(cap), size(0)
{
    buckets = new Node*[capacity];
    for (int i = 0; i < capacity; ++i)
        buckets[i] = nullptr;
}

HashTableChaining::~HashTableChaining() {
    clear();
    delete[] buckets;
}

int HashTableChaining::hash(int key) const {
    return ((key % capacity) + capacity) % capacity;
}

void HashTableChaining::insert(int key, int value) {
    int idx = hash(key);

    // sprawdzamy czy klucz już istnieje, jak tak to aktualizujemy
    Node* curr = buckets[idx];
    while (curr != nullptr) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }

    // wstawiamy na początku listy (O(1))
    Node* newNode  = new Node(key, value);
    newNode->next  = buckets[idx];
    buckets[idx]   = newNode;
    ++size;
}

bool HashTableChaining::remove(int key) {
    int   idx  = hash(key);
    Node* curr = buckets[idx];
    Node* prev = nullptr;

    while (curr != nullptr) {
        if (curr->key == key) {
            if (prev == nullptr)
                buckets[idx] = curr->next;  // usuwamy głowę
            else
                prev->next = curr->next;    // usuwamy środek/koniec

            delete curr;
            --size;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

int* HashTableChaining::search(int key) const {
    int   idx  = hash(key);
    Node* curr = buckets[idx];

    while (curr != nullptr) {
        if (curr->key == key)
            return &curr->value;
        curr = curr->next;
    }
    return nullptr;
}

void HashTableChaining::clear() {
    for (int i = 0; i < capacity; ++i) {
        Node* curr = buckets[i];
        while (curr != nullptr) {
            Node* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
        buckets[i] = nullptr;
    }
    size = 0;
}

int HashTableChaining::getSize()     const { return size; }
int HashTableChaining::getCapacity() const { return capacity; }

void HashTableChaining::print() const {
    std::cout << "HashTableChaining (size=" << size << ", capacity=" << capacity << ")\n";
    for (int i = 0; i < capacity; ++i) {
        if (buckets[i] == nullptr) continue;
        std::cout << "  [" << i << "]: ";
        Node* curr = buckets[i];
        while (curr != nullptr) {
            std::cout << "(" << curr->key << "," << curr->value << ")";
            if (curr->next) std::cout << " -> ";
            curr = curr->next;
        }
        std::cout << "\n";
    }
}