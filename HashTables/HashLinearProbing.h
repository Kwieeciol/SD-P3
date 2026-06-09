#pragma once

enum class SlotState { EMPTY, OCCUPIED, DELETED };

struct Slot {
    int       key;
    int       value;
    SlotState state;

    Slot();
};

class HashTableLinearProbing {
private:
    Slot* slots;
    int   capacity;
    int   size;

    int hash(int key) const;

public:
    explicit HashTableLinearProbing(int cap);
    ~HashTableLinearProbing();

    void insert(int key, int value);
    bool remove(int key);
    int* search(int key) const;

    void clear();

    int getSize()     const;
    int getCapacity() const;

    void print() const;
};