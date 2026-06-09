#include "HashLinearProbing.h"
#include <iostream>

Slot::Slot() : key(0), value(0), state(SlotState::EMPTY) {}

HashTableLinearProbing::HashTableLinearProbing(int cap)
    : capacity(cap), size(0)
{
    slots = new Slot[capacity];
}

HashTableLinearProbing::~HashTableLinearProbing() {
    delete[] slots;
}

int HashTableLinearProbing::hash(int key) const {
    return ((key % capacity) + capacity) % capacity;
}

void HashTableLinearProbing::insert(int key, int value) {
    int idx          = hash(key);
    int firstDeleted = -1;

    for (int i = 0; i < capacity; ++i) {
        int probe = (idx + i) % capacity;

        if (slots[probe].state == SlotState::OCCUPIED && slots[probe].key == key) {
            slots[probe].value = value;
            return;
        }

        if (slots[probe].state == SlotState::DELETED && firstDeleted == -1)
            firstDeleted = probe;

        if (slots[probe].state == SlotState::EMPTY) {
            int target = (firstDeleted != -1) ? firstDeleted : probe;
            slots[target].key   = key;
            slots[target].value = value;
            slots[target].state = SlotState::OCCUPIED;
            ++size;
            return;
        }
    }

    if (firstDeleted != -1) {
        slots[firstDeleted].key   = key;
        slots[firstDeleted].value = value;
        slots[firstDeleted].state = SlotState::OCCUPIED;
        ++size;
    }
}

bool HashTableLinearProbing::remove(int key) {
    int idx = hash(key);

    for (int i = 0; i < capacity; ++i) {
        int probe = (idx + i) % capacity;

        if (slots[probe].state == SlotState::EMPTY)
            return false;  // klucz nie istnieje

        if (slots[probe].state == SlotState::OCCUPIED && slots[probe].key == key) {
            slots[probe].state = SlotState::DELETED;
            --size;
            return true;
        }
    }
    return false;
}

int* HashTableLinearProbing::search(int key) const {
    int idx = hash(key);

    for (int i = 0; i < capacity; ++i) {
        int probe = (idx + i) % capacity;

        if (slots[probe].state == SlotState::EMPTY)
            return nullptr;  // klucza nie ma

        if (slots[probe].state == SlotState::OCCUPIED && slots[probe].key == key)
            return &slots[probe].value;
    }
    return nullptr;
}

void HashTableLinearProbing::clear() {
    for (int i = 0; i < capacity; ++i)
        slots[i].state = SlotState::EMPTY;
    size = 0;
}

int HashTableLinearProbing::getSize()     const { return size; }
int HashTableLinearProbing::getCapacity() const { return capacity; }

void HashTableLinearProbing::print() const {
    std::cout << "HashTableLinearProbing (size=" << size << ", capacity=" << capacity << ")\n";
    for (int i = 0; i < capacity; ++i) {
        if (slots[i].state == SlotState::EMPTY) continue;
        std::cout << "  [" << i << "]: ";
        if (slots[i].state == SlotState::DELETED)
            std::cout << "<DELETED>";
        else
            std::cout << "(" << slots[i].key << "," << slots[i].value << ")";
        std::cout << "\n";
    }
}