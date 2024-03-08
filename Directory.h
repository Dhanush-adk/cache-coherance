#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#include <unordered_map>
#include <vector>
#include "cache.h"

using namespace std;

class Directory
{
private:
    typedef struct directory
    {
        uint8_t cpu_id;
        cache_state state;
        vector<string> sharers;
    }directory_t;

public:
    unordered_map< uint32_t, directory_t > directory_map;
};


#endif //__DIRECTORY_H__