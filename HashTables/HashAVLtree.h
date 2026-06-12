#pragma once

// Węzeł drzewa AVL przechowujący parę klucz-wartość oraz wysokość drzewa do balansowania
struct AVLNode {
    int key;
    int value;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int k, int v);
};

// Klasa pomocnicza reprezentująca drzewo AVL dla pojedynczego kubełka
class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node) const;
    int getBalanceFactor(AVLNode* node) const;

    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);

    AVLNode* insert(AVLNode* node, int key, int value);
    AVLNode* remove(AVLNode* node, int key);
    AVLNode* minValueNode(AVLNode* node) const;

    int* search(AVLNode* node, int key) const;
    void clear(AVLNode* node);
    void print(AVLNode* node) const;

public:
    AVLTree();
    ~AVLTree();

    void insert(int key, int value);
    bool remove(int key);
    int* search(int key) const;
    void clear();
    void print() const;
};

// Właściwa klasa Tablicy Mieszającej opartej o drzewa AVL
class HashTableAVL {
private:
    AVLTree* buckets; // Tablica obiektów drzew AVL (kubełków)
    int capacity;
    int size;

    int hash(int key) const;

public:
    explicit HashTableAVL(int cap);
    ~HashTableAVL();

    void insert(int key, int value); // Wywołuje insert() na odpowiednim drzewie [cite: 18]
    bool remove(int key);            // Wywołuje remove() na odpowiednim drzewie 
    int* search(int key) const;

    void clear();

    int getSize()     const;
    int getCapacity() const;

    void print() const;
};