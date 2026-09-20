#include <vector>

#include "Parser.h"
#include "Calculator.h"

class Calculator
{
public:
    static double calculateLoad(const std::vector<CpuCore>& coresOldStats, const std::vector<CpuCore>& coresNewStats)
    {
        unsigned long long idle = 0, busy = 0;
            idle = (coresNewStats.idle - coresOldStats.idle) + (coresNewStats.iowait - coresOldStats.iowait);
            busy = (coresNewStats.user - coresOldStats.user) +
                       (coresNewStats.nice - coresOldStats.nice) +
                       (coresNewStats.system - coresOldStats.system) +
                       (coresNewStats.irq - coresOldStats.irq) +
                       (coresNewStats.softirq - coresOldStats.softirq) +
                       (coresNewStats.steal - coresOldStats.steal);
        const auto total = idle + busy;

        if (total == 0)
            return 0.0;

        return static_cast<double>(busy) / total * 100.0;
    }
};
