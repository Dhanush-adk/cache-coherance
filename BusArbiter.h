// busarbiter.h
#ifndef BUS_ARBITER_H
#define BUS_ARBITER_H

#include <queue>
#include <mutex>
#include "MESI.h"

class MESICacheSimulator;  // Forward declaration

enum MESIAction { CACHE_READ, CACHE_WRITE };
struct BusRequest {
    int processorID;
    int blockAddress;
    MESIAction action;
};

class BusArbiter {
private:
    std::queue<BusRequest> requestQueue;
    std::mutex mtx;
    MESICacheSimulator& simulator;  // Reference to the MESICacheSimulator object

public:
    BusArbiter(MESICacheSimulator& sim);  
    int requestBus(int processorID, int blockAddress, MESIAction action);
    int sendInvalidation(int senderProcessorID, int blockAddress);
    int sendCopy(int ProcessorID, int blockAddress, int data);
    void processRequest();
};

#endif // BUS_ARBITER_H
