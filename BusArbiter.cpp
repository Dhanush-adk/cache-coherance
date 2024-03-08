// busarbiter.cpp
#include "BusArbiter.h"
#include "MESICacheSimulator.h"
#include <iostream>

BusArbiter::BusArbiter(MESICacheSimulator& sim) : simulator(sim) {}

int BusArbiter::requestBus(int processorID, int blockAddress, MESIAction action) {
    std::lock_guard<std::mutex> lock(mtx);
    requestQueue.push({ processorID, blockAddress, action });
    std::cout << "Processor " << processorID << " requested the bus for address " << blockAddress << std::endl;
    std::cout << "Number of processors waiting for bus: " << requestQueue.size() << std::endl;
    return 1;
}

int BusArbiter::sendInvalidation(int senderProcessorID, int blockAddress) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Processor " << senderProcessorID << " receives invalidation for address " << blockAddress << std::endl;
    return 1;
}

int BusArbiter::sendCopy(int ProcessorID, int blockAddress, int data) {
    std::lock_guard<std::mutex> lock(mtx);
    simulator.memory.data[blockAddress] = data;
    std::cout << "Processor " << ProcessorID << " received a copy for address " << blockAddress << std::endl;
    return 1;
}


void BusArbiter::processRequest() {
    std::cout << "Number of processors waiting for bus: " << requestQueue.size() << std::endl;
    if (!requestQueue.empty()) {
        BusRequest request = requestQueue.front();
        requestQueue.pop();
        std::cout << "Processor " << request.processorID << " is granted the bus for address " << request.blockAddress << std::endl;
        std::cout << "Processing request from Processor " << request.processorID << " for address " << request.blockAddress << std::endl;
    }
    else {
        std::cout << "No processors waiting for the bus." << std::endl;
    }    
}

