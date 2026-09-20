#pragma once


#include <vector>


struct CpuCore
{
    unsigned int number;
    unsigned long long prev[8];
    unsigned long long curr[8];
    double load;
};

class Parser
{
public:
    static std::vector<CpuCore> parseAll();
    static void updateAll(std::vector<CpuCore>& cpu_cores);
};
