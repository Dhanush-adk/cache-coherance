#ifndef MESI_CACHE_SIMULATOR_H
#define MESI_CACHE_SIMULATOR_H

#include <vector>
#include "CacheMemoryStructs.h"
#include "BusArbiter.h"

class BusArbiter; 

class MESICacheSimulator {
private:
    static const int MEMORY_SIZE = 4096; // 4KB memory size
    int numProcessors;
    int cacheSizeKB;
    int blockSizeBytes;
    std::vector<Cache> caches;
    std::vector<int> ClockCyle;
    void initializeMemory();
    void initializeCaches();
    void processLocalAccess(int processorID, int blockAddress, MESIAction request);
    void processRemoteAccess(int RemoteProcessorID, int blockAddress, MESIAction request);
    void handleCacheMiss(int processorID, int blockAddress, MESIAction action);
    std::string stateToString(MESI state);

public:
    BusArbiter busArbiter;
    Memory memory;
    MESICacheSimulator(int numProcs, int cacheSize, int blockSize);
    void SimulateProcessorAccess(int processorID, int address, MESIAction request);

    void printClockCyle();
};

#endif 
