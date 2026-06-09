#include "HashChaining.h"
#include "HashLinearProbing.h"


int main() {
    HashTableChaining ht(8);
    HashTableLinearProbing hl(8);

    ht.insert(1, 10);
    ht.print();
    ht.remove(1);

    hl.insert(1, 10);
    hl.print();
    hl.remove(1);

    return 0;
}