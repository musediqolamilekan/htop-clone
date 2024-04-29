#include "CPUInfo.h"  
#include <iostream>

int main() {
    std::cout << "Processor Information:" << std::endl;
    auto processors = CPUInfo::GetProcessorInfo();
    for (const auto& proc : processors) {
        std::cout << "Processor ID: " << proc.processorId
                  << ", Model: " << proc.modelName
                  << ", User Mode Usage: " << proc.userModeUsage * 100 << "%"
                  << ", System Mode Usage: " << proc.systemModeUsage * 100 << "%"
                  << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Memory Information:" << std::endl;
    CPUInfo::printMemoryInfo(); 
    return 0;
}
