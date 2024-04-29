#ifndef CPUINFO_H
#define CPUINFO_H

#include <vector>
#include <string>

struct ProcessorInfo {
    int processorId;
    std::string modelName;
    double userModeUsage;
    double systemModeUsage;
};

class CPUInfo {
public:
    static std::vector<ProcessorInfo> GetProcessorInfo();
    static double GetTotalMemoryUsageMB();
    static void printMemoryInfo();
};

#endif 
