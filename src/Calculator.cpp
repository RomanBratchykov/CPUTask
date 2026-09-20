#include "Calculator.h"


double Calculator::calculateLoad(const CpuCore& coreOldStats, const CpuCore& coreNewStats)
{
    unsigned long long idle = 0, busy = 0;
        idle = (coreNewStats.idle - coreOldStats.idle) + (coreNewStats.iowait - coreOldStats.iowait);
        busy = (coreNewStats.user - coreOldStats.user) +
                   (coreNewStats.nice - coreOldStats.nice) +
                   (coreNewStats.system - coreOldStats.system) +
                   (coreNewStats.irq - coreOldStats.irq) +
                   (coreNewStats.softirq - coreOldStats.softirq) +
                   (coreNewStats.steal - coreOldStats.steal);
    const auto total = idle + busy;

    if (total == 0)
        return 0.0;

    return static_cast<double>(busy) / total * 100.0;
}

std::vector<CpuCore> Calculator::acceptLoad(std::vector<CpuCore>& oldStats, std::vector<CpuCore>& newStats)
{
    for (size_t i = 0; i < oldStats.size(); ++i) {
        newStats[i].load = calculateLoad(oldStats[i], newStats[i]);
    }
    return newStats;
}

