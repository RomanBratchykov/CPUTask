#include "Parser.h"
#include "Calculator.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iterator>

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

    if (!(input >> core.curr[0] >> core.curr[1] >> core.curr[2] >> core.curr[3]
                >> core.curr[4] >> core.curr[5] >> core.curr[6] >> core.curr[7]))
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
            std::copy(std::begin(core.curr), std::end(core.curr), std::begin(core.prev));
            cores.push_back(core);
        }
    }

    return cores;
}

void Parser::updateAll(std::vector<CpuCore>& cpu_cores)
{
    std::ifstream procStat("/proc/stat");
    if (!procStat) throw std::runtime_error("Unable to open /proc/stat");

    std::string line;
    while (std::getline(procStat, line))
    {
        CpuCore tmp{};
        if (parseCpuLine(line, tmp))
        {
            std::copy(std::begin(cpu_cores[tmp.number].curr), std::end(cpu_cores[tmp.number].curr), std::begin(cpu_cores[tmp.number].prev)); // shift
            std::copy(std::begin(tmp.curr), std::end(tmp.curr), std::begin(cpu_cores[tmp.number].curr)); // new values
            cpu_cores[tmp.number].load = Calculator::calculateLoad(cpu_cores[tmp.number]);
        }
    }
}

