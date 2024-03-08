#ifndef CACHE_MEMORY_STRUCTS_H
#define CACHE_MEMORY_STRUCTS_H

#include <vector>
#include "MESI.h"

struct Cache {
	std::vector<CacheBlock> blocks;
};

struct Memory {
	std::vector<int> data;
};

#endif // CACHE_MEMORY_STRUCTS_H
