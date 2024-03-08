#include <iostream>
#include "MESICacheSimulator.h"
#include "BusArbiter.h"

int main() {
    const int numProcessors = 4;
    const int cacheSizeKB = 64;
    const int blockSizeB = 64;

    MESICacheSimulator simulator(numProcessors, cacheSizeKB, blockSizeB);

    for (int cycle = 0; cycle < 2; ++cycle) {
        std::cout << "cycle -- " << cycle  << "\n" << std::endl;
        for (int processorID = 0; processorID < numProcessors; ++processorID) {
            int address = processorID * 64;
            MESIAction request = CACHE_READ;
            simulator.SimulateProcessorAccess(processorID, address, request);
        }
    //  busArbiter.processRequest();
    }

    std::cout << "\n \n \n";
    simulator.SimulateProcessorAccess(0, 0, CACHE_READ);

    simulator.SimulateProcessorAccess(1, 64, CACHE_READ);

    simulator.SimulateProcessorAccess(1, 0, CACHE_READ);

    simulator.SimulateProcessorAccess(1, 0, CACHE_WRITE);

    simulator.SimulateProcessorAccess(2, 128, CACHE_READ);

    simulator.SimulateProcessorAccess(3, 192, CACHE_READ);

    simulator.SimulateProcessorAccess(0, 0, CACHE_WRITE);

    simulator.SimulateProcessorAccess(1, 64, CACHE_WRITE);

    simulator.SimulateProcessorAccess(1, 64, CACHE_READ);

    simulator.SimulateProcessorAccess(2, 64, CACHE_READ);

    simulator.SimulateProcessorAccess(2, 64, CACHE_WRITE);

    simulator.SimulateProcessorAccess(0, 192, CACHE_READ);

    simulator.SimulateProcessorAccess(1, 192, CACHE_READ);

    simulator.SimulateProcessorAccess(1, 192, CACHE_WRITE);

    simulator.SimulateProcessorAccess(2, 192, CACHE_READ);

    simulator.printClockCyle();

    return 0;
}
