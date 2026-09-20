#include "Printer.h"
#include "Calculator.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <iomanip>

void Printer::printToFile(std::filesystem::path const& path, int interval, std::vector<CpuCore>& cpu_cores, volatile sig_atomic_t const& running)
{
    if (interval <= 0)
    {
        throw std::invalid_argument("Interval must be greater than zero");
    }

    std::ofstream file(path, std::ios::app);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file for writing");
    }

    while (running)
    {
        printf("Writing to file...\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        Parser::updateAll(cpu_cores);

        for (const auto& cpu_core : cpu_cores)
        {
            file << "Core Name: cpu" << cpu_core.number << " Core Load: " << cpu_core.load << "%" << std::endl;
        }
        file << std::endl;
    }
    file.close();
}
void Printer::printToFile(std::filesystem::path const& path, std::vector<CpuCore>& cpu_cores, std::string const& time)
{
    std::ofstream file(path, std::ios::app);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file for writing");
    }

    printf("Writing to file...\n");
    file << "Current time: " << time << std::endl;
    for (const auto& cpu_core : cpu_cores)
    {
        file << "Core Name: cpu" << cpu_core.number << " Core Load: " << std::fixed << std::setprecision(2) << cpu_core.load << "%" << std::endl;
    }
    file << std::endl;

    file.close();
}

void Printer::printToFile(std::filesystem::path const& path, std::vector<CpuCore>& cpu_cores, std::string const& time, int core)
{
    std::ofstream file(path, std::ios::app);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file for writing");
    }

    printf("Writing to file...\n");

    file << "Current time: " << time << std::endl;

    file << "Core Name: cpu" << cpu_cores[core].number << " Core Load: " << std::fixed << std::setprecision(2) << cpu_cores[core].load << "%" << std::endl;

    file << std::endl;
    file.close();

}

void Printer::printToConsole(std::vector<CpuCore>& cpu_cores, std::string const& time)
{
    std::cout << "Printing to console..." << std::endl;
    std::cout << "Current time: " << time << std::endl;
    for (const auto& cpu_core : cpu_cores)
    {
        std::cout << "Core Name: cpu" << cpu_core.number << " Core Load: " << std::fixed << std::setprecision(2) << cpu_core.load << "%" << std::endl;
    }
}
void Printer::printToConsoleSpecific(std::vector<CpuCore>& cpu_cores, std::string const& time, int core)
{
    std::cout << "Printing to console..." << std::endl;
    std::cout << "Current time: " << time << std::endl;
    std::cout << "Core Name: cpu" << cpu_cores[core].number << " Core Load: " << std::fixed << std::setprecision(2) << cpu_cores[core].load << "%" << std::endl;
}
