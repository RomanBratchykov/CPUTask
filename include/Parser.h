#pragma once

#include <cstdint>
#include <vector>

struct CpuCore
{
    unsigned int number;
    std::uint64_t user;
    std::uint64_t nice;
    std::uint64_t system;
    std::uint64_t idle;
    std::uint64_t iowait;
    std::uint64_t irq;
    std::uint64_t softirq;
    std::uint64_t steal;
};

class Parser
{
public:
    static std::vector<CpuCore> parseAll();
};
