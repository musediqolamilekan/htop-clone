#include "CPUInfo.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <iterator>
#include <iomanip>

void CPUInfo::printMemoryInfo() {
    std::ifstream meminfo("/proc/meminfo");
    std::string key, line, unit;
    long value;

    std::cout << std::left << std::setw(20) << "Memory Key" 
              << std::setw(15) << "Value (kB)" 
              << std::setw(15) << "Value (MB)" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        iss >> key >> value >> unit;
        key = key.substr(0, key.length() - 1);
        std::cout << std::left << std::setw(20) << key 
                  << std::setw(15) << value 
                  << std::setw(15) << value / 1024.0 << std::endl; 
    }
}


std::vector<ProcessorInfo> CPUInfo::GetProcessorInfo() {
    std::vector<ProcessorInfo> processors;
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    int processorId = -1;
    std::string modelName;

    while (std::getline(cpuinfo, line)) {
        if (line.substr(0, 9) == "processor") {
            if (processorId != -1) { 
                processors.push_back({processorId, modelName, 0.0, 0.0});
            }
            processorId = stoi(line.substr(line.find(":") + 1));
        } else if (line.substr(0, 10) == "model name") {
            modelName = line.substr(line.find(":") + 2);
        }
    }
    if (processorId != -1) { 
        processors.push_back({processorId, modelName, 0.0, 0.0});
    }

    std::ifstream procstat("/proc/stat");
    std::string statLine;
    std::getline(procstat, statLine); 
    for (auto& proc : processors) {
        std::getline(procstat, statLine);
        std::istringstream iss(statLine);
        std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
        long user = std::stol(tokens[1]);
        long nice = std::stol(tokens[2]);
        long system = std::stol(tokens[3]);
        long idle = std::stol(tokens[4]);
        long total = user + nice + system + idle;
        proc.userModeUsage = static_cast<double>(user + nice) / total;
        proc.systemModeUsage = static_cast<double>(system) / total;
    }

    return processors;
};

double CPUInfo::GetTotalMemoryUsageMB() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    long totalMemory = 0;
    long freeMemory = 0;
    long availableMemory = 0;

    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;
        std::string unit;
        iss >> key >> value >> unit;  // Read key, value, and unit

        if (key == "MemTotal:") {
            totalMemory = value;
        } else if (key == "MemFree:") {
            freeMemory = value;
        } else if (key == "MemAvailable:") {
            availableMemory = value;
        }
    }
    
    long usedMemory = totalMemory - freeMemory;  // Calculate used memory
    return usedMemory / 1024.0;  // Convert from KB to MB
}