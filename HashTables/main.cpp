#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

// Nagłówki Twoich struktur danych
#include "HashLinearProbing.h"
#include "HashChaining.h"
#include "HashAVLTree.h"

enum Operation {
    INSERT, REMOVE
};

std::string getOpName(Operation op) {
    switch (op) {
    case INSERT: return "Insert";
    case REMOVE: return "Remove";
    default: return "Unknown";
    }
}

std::mt19937& getGenerator(unsigned int seed) {
    static std::mt19937 gen(seed);
    static unsigned int currentSeed = seed;
    if (seed != currentSeed) {
        gen.seed(seed);
        currentSeed = seed;
    }
    return gen;
}

// Funkcja generująca plik tekstowy z danymi testowymi (klucz wartość)
void generateHashFile(unsigned int seed, int size, std::string fileName) {
    std::ofstream file(fileName);
    if (!file.is_open()) return;

    auto& gen = getGenerator(seed);
    std::uniform_int_distribution<int> dist(-size * 5, size * 5); // Zakres dopuszczający liczby ujemne

    file << size << "\n";
    for (int i = 0; i < size; i++) {
        file << dist(gen) << " " << dist(gen) << "\n"; // Losowy klucz i losowa wartość
    }
    file.close();
    std::cout << "Wygenerowano plik '" << fileName << "' przy uzyciu mt19937." << std::endl;
}

// Szablon realizujący badanie czasu wykonania pojedynczej operacji
template <typename T>
long long measureHash(int size, Operation op, unsigned int seed, int repetitions) {
    long long totalDuration = 0;
    auto& gen = getGenerator(seed);

    std::uniform_int_distribution<int> dist(-size * 5, size * 5);

    for (int r = 0; r < repetitions; r++) {
        // Inicjalizujemy tablicę z pojemnością równą docelowemu rozmiarowi badania
        T* ht = new T(size);
        std::vector<int> insertedKeys;
        insertedKeys.reserve(size);

        // 1. Wstępne zapełnienie struktury do rozmiaru bazowego N
        for (int j = 0; j < size; j++) {
            int k = dist(gen);
            int v = dist(gen);
            ht->insert(k, v);
            insertedKeys.push_back(k);
        }

        // Przygotowanie elementów do testów właściwych
        int randomKey = dist(gen);
        int randomValue = dist(gen);
        int keyToRemove = 0;

        if (op == REMOVE && !insertedKeys.empty()) {
            std::uniform_int_distribution<int> indexDist(0, (int)insertedKeys.size() - 1);
            keyToRemove = insertedKeys[indexDist(gen)];
        }

        // 2. Pomiar czasu właściwej operacji
        auto start = std::chrono::high_resolution_clock::now();

        if (op == INSERT) {
            ht->insert(randomKey, randomValue);
        }
        else if (op == REMOVE) {
            ht->remove(keyToRemove);
        }

        auto end = std::chrono::high_resolution_clock::now();

        totalDuration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        delete ht;
    }
    return totalDuration / repetitions;
}

// Główna funkcja wykonująca automatyczną serię badań testowych
void runHashTests(const unsigned int seeds[10]) {
    std::vector<Operation> ops = { INSERT, REMOVE };

    for (auto op : ops) {
        std::string fileName = "test_hash_" + getOpName(op) + ".csv";
        std::ofstream file(fileName);
        file << "Rozmiar;LinearProbing [ns];Chaining [ns];AVLTree [ns]\n";

        std::cout << "\nBadanie operacji: " << getOpName(op) << "..." << std::endl;

        // 8 progów rozmiarowych: od 5 000 do 40 000 co 5 000 elementów
        for (int i = 1; i <= 8; i++) {
            int N = i * 5000;
            long long avgLinear = 0, avgChaining = 0, avgAVL = 0;

            for (int s = 0; s < 10; s++) { // 10 niezależnych serii danych (seedów)
                std::cout << "\rRozmiar: " << std::setw(6) << N
                    << " | Postep: [" << std::setw(2) << s + 1 << "/10] | Linear... " << std::flush;
                avgLinear += measureHash<HashTableLinearProbing>(N, op, seeds[s], 10);

                std::cout << "OK | Chaining... " << std::flush;
                avgChaining += measureHash<HashTableChaining>(N, op, seeds[s], 10);

                std::cout << "OK | AVLTree... " << std::flush;
                avgAVL += measureHash<HashTableAVL>(N, op, seeds[s], 10);
                std::cout << "OK" << std::flush;
            }

            // Zapis uśrednionych wyników ze wszystkich seedów do pliku CSV
            file << N << ";" << avgLinear / 10 << ";" << avgChaining / 10 << ";" << avgAVL / 10 << "\n";
        }
        file.close();
        std::system("cls");
    }
    std::cout << "Badania zakonczone sukcesem! Wyniki zapisano do plikow .csv\n";
}

// Menu operacji interaktywnych na konkretnej strukturze
template <typename T>
void hashMenu(std::string name) {
    int initialCap = 16;
    std::cout << "Podaj poczatkowa pojemnosc tablicy (capacity) dla " << name << ": ";
    std::cin >> initialCap;
    std::system("cls");

    T ht(initialCap);
    int choice = -1;
    std::random_device rd;
    std::mt19937 gen(rd());

    while (choice != 0) {
        std::cout << "\n--- " << name << " (Rozmiar: " << ht.getSize() << ", Pojemnosc: " << ht.getCapacity() << ") ---" << std::endl;
        std::cout << "1. Zbuduj z pliku\n2. Losowe wypelnienie\n3. Insert (Wstaw)\n4. Remove (Usun)\n5. Search (Szukaj)\n6. Czyszczenie (Clear)\n7. Drukuj tablice (Print)\n0. Powrot\nWybor: ";
        if (!(std::cin >> choice)) { std::cin.clear(); while (std::cin.get() != '\n'); continue; }
        std::system("cls");

        switch (choice) {
        case 1: {
            std::string path; std::cout << "Sciezka do pliku: "; std::cin >> path;
            std::ifstream f(path);
            if (f.is_open()) {
                int n, k, v; f >> n;
                while (f >> k >> v) ht.insert(k, v);
                std::cout << "Wczytano dane z pliku.\n";
            }
            else {
                std::cout << "Nie udalo sie otworzyc pliku!\n";
            }
            break;
        }
        case 2: {
            int n; std::cout << "Ile losowych elementow dodac: "; std::cin >> n;
            std::uniform_int_distribution<int> d(-n * 5, n * 5);
            for (int i = 0; i < n; i++) ht.insert(d(gen), d(gen));
            std::cout << "Dodano " << n << " losowych par.\n";
            break;
        }
        case 3: {
            int k, v;
            std::cout << "Klucz (int): "; std::cin >> k;
            std::cout << "Wartosc (int): "; std::cin >> v;
            ht.insert(k, v);
            std::cout << "Wstawiono pare (" << k << ", " << v << ").\n";
            break;
        }
        case 4: {
            int k; std::cout << "Podaj klucz do usuniecia: "; std::cin >> k;
            if (ht.remove(k)) {
                std::cout << "Prybnie usunieto klucz " << k << ".\n";
            }
            else {
                std::cout << "Klucz " << k << " nie istnieje w tablicy.\n";
            }
            break;
        }
        case 5: {
            int k; std::cout << "Podaj klucz do wyszukania: "; std::cin >> k;
            int* res = ht.search(k);
            if (res != nullptr) {
                std::cout << "Znaleziono! Wartosc pod kluczem " << k << " to: " << *res << "\n";
            }
            else {
                std::cout << "Klucz " << k << " nie istnieje.\n";
            }
            break;
        }
        case 6: {
            ht.clear();
            std::cout << "Tablica zostala wyczyszczona.\n";
            break;
        }
        case 7: {
            ht.print();
            break;
        }
        }
    }
    std::system("cls");
}

int main() {
    const unsigned int seeds[10] = { 12345, 67890, 11223, 44556, 77889, 99001, 13579, 24680, 98765, 54321 };
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n=== BADANIE TABLIC MIESZAJACYCH ===\n";
        std::cout << "1. Tablica z probkowaniem liniowym (Linear Probing)\n";
        std::cout << "2. Tablica z metoda lancuchowa (Lista - Chaining)\n";
        std::cout << "3. Tablica z metoda lancuchowa (Drzewo AVL - HashAVL)\n";
        std::cout << "4. Wygeneruj plik testowy\n";
        std::cout << "5. Badania porownawcze (Generowanie plikow .csv)\n";
        std::cout << "0. Wyjscie\nWybor: ";
        if (!(std::cin >> choice)) { std::cin.clear(); while (std::cin.get() != '\n'); continue; }
        std::system("cls");

        switch (choice) {
        case 1: hashMenu<HashTableLinearProbing>("Linear Probing"); break;
        case 2: hashMenu<HashTableChaining>("Hash Chaining (List)"); break;
        case 3: hashMenu<HashTableAVL>("Hash AVL Tree"); break;
        case 4: {
            int n; std::string name;
            std::cout << "Liczba par (N): "; std::cin >> n;
            std::cout << "Nazwa pliku wyjsciowego: "; std::cin >> name;
            generateHashFile(seeds[0], n, name);
            break;
        }
        case 5: runHashTests(seeds); break;
        }
    }
    return 0;
}