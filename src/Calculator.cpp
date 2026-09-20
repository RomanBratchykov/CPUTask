#include "Calculator.h"


double Calculator::calculateLoad(const CpuCore& core)
{
    unsigned long long idle = (core.curr[3] - core.prev[3]) + (core.curr[4] - core.prev[4]);
    unsigned long long busy = (core.curr[0] - core.prev[0]) + (core.curr[1] - core.prev[1])
                            + (core.curr[2] - core.prev[2]) + (core.curr[5] - core.prev[5])
                            + (core.curr[6] - core.prev[6]) + (core.curr[7] - core.prev[7]);
    unsigned long long total = idle + busy;
    if (total == 0) return 0.0;
    return static_cast<double>(busy) / total * 100.0;
}


