#include "MESICacheSimulator.h"
#include "BusArbiter.h"

#include <iostream>

MESICacheSimulator::MESICacheSimulator(int numProcs, int cacheSize, int blockSize)
    : numProcessors(numProcs), cacheSizeKB(cacheSize), blockSizeBytes(blockSize), busArbiter(*this) {
    initializeMemory();
    initializeCaches();
    ClockCyle.assign(numProcessors, 0);
}

void MESICacheSimulator::initializeMemory() {
    memory.data.assign(MEMORY_SIZE, 0); 
}

void MESICacheSimulator::initializeCaches() {
    caches = std::vector<Cache>(numProcessors); 
    for (Cache& cache : caches) {
        for (int i = 0; i < cacheSizeKB * 1024 / blockSizeBytes; ++i) {
            cache.blocks.push_back(CacheBlock{0, INVALID }); 
        }
    }
}

void MESICacheSimulator::SimulateProcessorAccess(int processorID, int address, MESIAction request) {
    int blockOffset = address % blockSizeBytes;
    int blockAddress = address - blockOffset;
    CacheBlock& cacheBlock = caches[processorID].blocks[blockAddress / blockSizeBytes];
    std::cout << "Processor " << processorID << ", Address " << address << ", Initial State: " << stateToString(cacheBlock.state) << std::endl;
    ClockCyle[processorID] += 1;

    bool isCacheMiss = (cacheBlock.state == INVALID);
    if (isCacheMiss) {
        ClockCyle[processorID] += 3;
        handleCacheMiss(processorID, blockAddress, request);
    }

    int busLatency = busArbiter.requestBus(processorID, blockAddress, request);
    ClockCyle[processorID] += 1; 

    if (!isCacheMiss) {
        processLocalAccess(processorID, blockAddress, request);
    }

    processRemoteAccess(processorID, blockAddress, request);

    std::cout << "Final State: " << stateToString(cacheBlock.state) << "\n \n";
}

void MESICacheSimulator::processLocalAccess(int processorID, int blockAddress, MESIAction request) {
    CacheBlock& cacheBlock = caches[processorID].blocks[blockAddress / blockSizeBytes];

    switch (request) {
    case CACHE_READ:
        if (cacheBlock.state != INVALID) {
            std::cout << "Read Hit for Cache Processor " << processorID << std::endl;
        } else {
            std::cout << "Read Miss for Cache Processor " << processorID << std::endl;
            handleCacheMiss(processorID, blockAddress, CACHE_READ);
        }
        break;

    case CACHE_WRITE:
        if (cacheBlock.state == MODIFIED) {
            std::cout << "Write Hit for Cache Processor " << processorID << " (Modified)" << std::endl;
        } else if (cacheBlock.state == EXCLUSIVE) {
            cacheBlock.state = MODIFIED;
            std::cout << "Write Hit for Cache Processor " << processorID << ", State Transition to MODIFIED" << std::endl;
        } else if (cacheBlock.state == SHARED) {
            cacheBlock.state = MODIFIED;
            std::cout << "Write Hit for Cache Processor " << processorID << ", Invalidate Others, State Transition to MODIFIED" << std::endl;
        } else {
            std::cout << "Write Miss for Cache Processor " << processorID << std::endl;
            handleCacheMiss(processorID, blockAddress, CACHE_WRITE);
            cacheBlock.state = MODIFIED;
        }
        break;
    }
}

void MESICacheSimulator::processRemoteAccess(int RemoteProcessorID, int blockAddress, MESIAction request) {
    bool isShared = false;
    bool isModified = false;
    for (int i = 0; i < numProcessors; ++i) {
        if (i != RemoteProcessorID) {
            CacheBlock& otherCacheBlock = caches[i].blocks[blockAddress / blockSizeBytes];
            if (otherCacheBlock.state == SHARED || otherCacheBlock.state == MODIFIED) {
                isShared = true;
            }
            if (request == CACHE_WRITE) {
                otherCacheBlock.state = INVALID;
                busArbiter.sendInvalidation(i, blockAddress);
            }
            else if (request == CACHE_READ && otherCacheBlock.state == EXCLUSIVE) {
                otherCacheBlock.state = SHARED;
                isShared = true; 
            }
            if (request == CACHE_READ && otherCacheBlock.state == MODIFIED) {
                    busArbiter.sendCopy(i, blockAddress, otherCacheBlock.data);
                    otherCacheBlock.state = SHARED;
               
            }
        }
    }

    CacheBlock& requestingCacheBlock = caches[RemoteProcessorID].blocks[blockAddress / blockSizeBytes];
    if (requestingCacheBlock.state != MODIFIED) {
        requestingCacheBlock.state = isShared ? SHARED : EXCLUSIVE;
    }
    else if (request == CACHE_WRITE) {
        requestingCacheBlock.state = MODIFIED;
    }
}

void MESICacheSimulator::handleCacheMiss(int processorID, int blockAddress, MESIAction action) {
    std::cout << "Cache miss for Cache Processor " << processorID << std::endl;
    if (action == CACHE_READ) {
        std::cout << "Read miss for Cache Processor " << processorID << std::endl;
    }
    if (action == CACHE_WRITE) {
        std::cout << "Write miss for Cache Processor " << processorID << std::endl;
    }

    CacheBlock& cacheBlock = caches[processorID].blocks[blockAddress / blockSizeBytes];
    cacheBlock.data = memory.data[blockAddress];
    bool isBlockShared = false;
    for (int i = 0; i < numProcessors; ++i) {
        if (i != processorID) {
            CacheBlock& otherCacheBlock = caches[i].blocks[blockAddress / blockSizeBytes];
            if (otherCacheBlock.state == SHARED || otherCacheBlock.state == MODIFIED) {
                isBlockShared = true;
                if (otherCacheBlock.state == MODIFIED) {
                    busArbiter.sendCopy(i, blockAddress, otherCacheBlock.data);
                    otherCacheBlock.state = SHARED;
                    std::cout << "Write-back to memory " << blockAddress << " in Cache Processor " << i << std::endl;
                }
            }
        }
    }

    if (action == CACHE_READ) {
        cacheBlock.state = isBlockShared ? SHARED : EXCLUSIVE;
    }
    else if (action == CACHE_WRITE) {
        cacheBlock.state = MODIFIED;
    }
}

std::string MESICacheSimulator::stateToString(MESI state) {
    switch (state) {
    case INVALID: return "Invalid";
    case SHARED: return "Shared";
    case EXCLUSIVE: return "Exclusive";
    case MODIFIED: return "Modified";
    default: return "Unknown";
    }
}

void MESICacheSimulator::printClockCyle() {
    for (int i = 0; i < numProcessors; ++i) {
        std::cout << "Processor " << i << " used " << ClockCyle[i] << " Clock Cycles." << std::endl;
    }
}


