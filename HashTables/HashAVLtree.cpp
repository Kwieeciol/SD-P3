#include "HashAVLTree.h"
#include <iostream>
// Funkcja pomocnicza do obliczania maksimum dwóch liczb
int c_max(int a, int b) {
    return (a > b) ? a : b;
}
// Funkcja pomocnicza do obliczania wartości bezwzględnej
int c_abs(int n) {
    return (n < 0) ? -n : n;
}
// Konstruktor węzła AVLNode
AVLNode::AVLNode(int k, int v)
    : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
// Konstruktor drzewa AVLTree
AVLTree::AVLTree() : root(nullptr) {}
// Destruktor drzewa AVLTree
AVLTree::~AVLTree() {
    clear(root);
}
// Pobranie wysokości węzła
int AVLTree::getHeight(AVLNode* node) const {
    return node ? node->height : 0;
}
// Pobranie współczynnika zbalansowania węzła
int AVLTree::getBalanceFactor(AVLNode* node) const {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}
// Rotacja w prawo
AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Rotacja
    x->right = y;
    y->left = T2;

    // Aktualizacja wysokości
    y->height = c_max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = c_max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}
// Rotacja w lewo
AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Rotacja
    y->left = x;
    x->right = T2;

    // Aktualizacja wysokości
    x->height = c_max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = c_max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Wstawianie węzła do drzewa AVL
AVLNode* AVLTree::insert(AVLNode* node, int key, int value) {
    if (!node) return new AVLNode(key, value);
    if (key < node->key)
        node->left = insert(node->left, key, value);
    else if (key > node->key)
        node->right = insert(node->right, key, value);
    else {
        node->value = value;
        return node;
    }

    // Aktualizacja wysokości bieżącego węzła
    node->height = 1 + c_max(getHeight(node->left), getHeight(node->right));

    // Pobranie współczynnika zbalansowania
    int balance = getBalanceFactor(node);

    // Przypadki braku zbalansowania (Rotacje)
    // Przypadek Lewo-Lewo
    if (balance > 1 && key < node->left->key)
        return rotateRight(node);

    // Przypadek Prawo-Prawo
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);

    // Przypadek Lewo-Prawo
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Przypadek Prawo-Lewo
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}
// Funkcja pomocnicza do znalezienia węzła o najmniejszym kluczu
AVLNode* AVLTree::minValueNode(AVLNode* node) const {
    AVLNode* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

// Usuwanie węzła z drzewa AVL
AVLNode* AVLTree::remove(AVLNode* node, int key) {
    if (!node) return node;

    if (key < node->key)
        node->left = remove(node->left, key);
    else if (key > node->key)
        node->right = remove(node->right, key);
    else {
        // Węzeł z jednym dzieckiem lub bez dzieci
        if (!node->left || !node->right) {
            AVLNode* temp = node->left ? node->left : node->right;

            if (!temp) { // Brak dzieci
                temp = node;
                node = nullptr;
            }
            else { // Jedno dziecko
                *node = *temp; // Kopiowanie zawartości
            }
            delete temp;
        }
        else {
            // Węzeł z dwoma synami: bierzemy następcę (najmniejszy w prawym poddrzewie)
            AVLNode* temp = minValueNode(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = remove(node->right, temp->key);
        }
    }

    if (!node) return node;

    // Aktualizacja wysokości
    node->height = 1 + c_max(getHeight(node->left), getHeight(node->right));

    // Sprawdzenie zbalansowania
    int balance = getBalanceFactor(node);

    // Rotacje naprawcze
    if (balance > 1 && getBalanceFactor(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && getBalanceFactor(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Wyszukiwanie klucza w drzewie AVL
int* AVLTree::search(AVLNode* node, int key) const {
    if (!node) return nullptr;
    if (node->key == key) return &node->value;
    if (key < node->key) return search(node->left, key);
    return search(node->right, key);
}
// Czyszczenie drzewa AVL
void AVLTree::clear(AVLNode* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}
// Wypisywanie drzewa AVL (in-order)
void AVLTree::print(AVLNode* node) const {
    if (!node) return;
    print(node->left);
    std::cout << "(" << node->key << "," << node->value << ") ";
    print(node->right);
}

// Metody publiczne wrappujące metody prywatne drzewa
void AVLTree::insert(int key, int value) { root = insert(root, key, value); }
bool AVLTree::remove(int key) {
    int* exists = search(root, key);
    if (!exists) return false;
    root = remove(root, key);
    return true;
}
int* AVLTree::search(int key) const { return search(root, key); }
void AVLTree::clear() { clear(root); root = nullptr; }
void AVLTree::print() const { print(root); }



// Konstruktor tablicy haszującej z drzewami AVL jako kubełkami
HashTableAVL::HashTableAVL(int cap) : capacity(cap), size(0) {
	buckets = new AVLTree[capacity]; // Alokacja pamięci dla tablicy kubełków 
}
// Destruktor tablicy haszującej
HashTableAVL::~HashTableAVL() {
    delete[] buckets; 
}
// Funkcja haszująca - prosta funkcja modulo
int HashTableAVL::hash(int key) const {
    return c_abs(key) % capacity;
}
// Wstawianie pary klucz-wartość do tablicy haszującej
void HashTableAVL::insert(int key, int value) {
    int idx = hash(key);

	// Sprawdzenie, czy klucz już istnieje w drzewie AVL kubełka
    int* exists = buckets[idx].search(key);
    buckets[idx].insert(key, value);

    if (!exists) {
        ++size; 
    }
}
// Usuwanie klucza z tablicy haszującej
bool HashTableAVL::remove(int key) {
    int idx = hash(key);
    bool deleted = buckets[idx].remove(key);
    if (deleted) {
        --size;
    }
    return deleted;
}
// Wyszukiwanie klucza w tablicy haszującej
int* HashTableAVL::search(int key) const {
    int idx = hash(key);
    return buckets[idx].search(key);
}
// Czyszczenie tablicy haszującej
void HashTableAVL::clear() {
    for (int i = 0; i < capacity; ++i) {
        buckets[i].clear();
    }
    size = 0;
}
// Pobranie rozmiaru tablicy haszującej
int HashTableAVL::getSize()     const { return size; }
// Pobranie pojemności tablicy haszującej
int HashTableAVL::getCapacity() const { return capacity; }
// Wypisywanie zawartości tablicy haszującej
void HashTableAVL::print() const {
    std::cout << "HashTableAVL (size=" << size << ", capacity=" << capacity << ")\n";
    for (int i = 0; i < capacity; ++i) {
        std::cout << "  [" << i << "]: ";
        buckets[i].print(); // Wypisze drzewo posortowane (in-order)
        std::cout << "\n";
    }
}