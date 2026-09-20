#pragma once

#include "Parser.h"

#include <vector>


class Calculator
{
    public:
        static double calculateLoad(const CpuCore& coreOldStats, const CpuCore& coreNewStats);

        static std::vector<CpuCore> acceptLoad(std::vector<CpuCore>& oldStats, std::vector<CpuCore>& newStats);
};
