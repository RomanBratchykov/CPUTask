#include "Printer.h"
#include "Calculator.h"

#include <iostream>


void Printer::printToFile(std::filesystem::path const& path, int interval)
{

}
void Printer::printToConsole(std::vector<CpuCore> const& cpu_cores, std::string const& time)
{
    std::cout << "Printing to console..." << std::endl;
    std::cout << "Current time: " << time << std::endl;
    for (const auto& core : cpu_cores)
    {
        std::cout << "Core Name: cpu" << core.number << std::endl;
        std::cout << "Core Load: " << core.load << std::endl;
    }
}
void Printer::printToConsoleSpecific(CpuCore const& cpu_core, std::string const& time)
{
    std::cout << "Printing to console..." << std::endl;
    std::cout << "Current time: " << time << std::endl;
    std::cout << "Core Name: cpu" << cpu_core.number << std::endl;
    std::cout << "Core Load: " << cpu_core.load << std::endl;
}
