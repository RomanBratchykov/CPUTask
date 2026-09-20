#include "Parser.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
bool parseCpuLine(std::string const& line, CpuCore& core)
{
    std::istringstream input(line);
    std::string name;

    if (!(input >> name) || name.size() <= 3 || name.compare(0, 3, "cpu") != 0)
    {
        return false;
    }

    std::string number = name.substr(3);
    for (char digit : number)
    {
        if (digit < '0' || digit > '9')
        {
            return false;
        }
    }

    try
    {
        core.number = static_cast<unsigned int>(std::stoul(number));
    }
    catch (std::exception const&)
    {
        throw std::runtime_error("Invalid CPU core number in /proc/stat");
    }

    if (!(input >> core.user >> core.nice >> core.system >> core.idle
                >> core.iowait >> core.irq >> core.softirq >> core.steal))
    {
        throw std::runtime_error("Invalid CPU counters in /proc/stat");
    }

    return true;
}
}

std::vector<CpuCore> Parser::parseAll()
{
    std::ifstream procStat("/proc/stat");
    if (!procStat)
    {
        throw std::runtime_error("Unable to open /proc/stat");
    }

    std::vector<CpuCore> cores;
    std::string line;
    while (std::getline(procStat, line))
    {
        CpuCore core{};
        if (parseCpuLine(line, core))
        {
            cores.push_back(core);
        }
    }

    return cores;
}

