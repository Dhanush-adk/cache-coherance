#ifndef MESI_H
#define MESI_H

// Cache line states
enum MESI { INVALID, SHARED, EXCLUSIVE, MODIFIED };

// Cache line structure
struct CacheBlock {
    int data;
    MESI state;
};

#endif // MESI_H
