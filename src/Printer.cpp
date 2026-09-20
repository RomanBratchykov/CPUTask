#include "Printer.h"
#include "Calculator.h"

#include <iostream>
#include <fstream>
#include <thread>


void Printer::printToFile(std::filesystem::path const& path, int interval)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file for writing");
    }

    while (true)
    {
        printf("Writing to file...\n");
        auto cores_first = Parser::parseAll();
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        auto cores_second = Parser::parseAll();
        auto cores = Calculator::acceptLoad(cores_first, cores_second);

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&time);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
        std::string timestr(buffer);

        file << "Current time: " << timestr << std::endl;
        for (const auto& cpu_core : cores)
        {
            file << "Core Name: cpu" << cpu_core.number << " Core Load: " << cpu_core.load << "%" << std::endl;
        }
        file << std::endl;
    }
}
void Printer::printToFile(std::filesystem::path const& path, std::vector<CpuCore> const& cpu_cores, std::string const& time)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file for writing");
    }
    if (file.is_open())
    {
        file << "Current time: " << time << std::endl;
        for (const auto& cpu_core : cpu_cores)
        {
            file << "Core Name: cpu" << cpu_core.number << " Core Load: " << cpu_core.load << "%" << std::endl;
        }
        file << std::endl;
    }
}

void Printer::printToFile(std::filesystem::path const& path, std::vector<CpuCore> const& cpu_cores, std::string const& time, int core)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file for writing");
    }
    if (file.is_open())
    {
        file << "Current time: " << time << std::endl;

        file << "Core Name: cpu" << cpu_cores[core].number << " Core Load: " << cpu_cores[core].load << "%" << std::endl;

        file << std::endl;
    }
}

void Printer::printToConsole(std::vector<CpuCore> const& cpu_cores, std::string const& time)
{
    std::cout << "Printing to console..." << std::endl;
    std::cout << "Current time: " << time << std::endl;
    for (const auto& cpu_core : cpu_cores)
    {
        std::cout << "Core Name: cpu" << cpu_core.number << " Core Load: " << cpu_core.load << "%" << std::endl;
    }
}
void Printer::printToConsoleSpecific(std::vector<CpuCore> const& cpu_cores, std::string const& time, int core)
{
    std::cout << "Printing to console..." << std::endl;
    std::cout << "Current time: " << time << std::endl;
    std::cout << "Core Name: cpu" << cpu_cores[core].number << " Core Load: " << cpu_cores[core].load << "%" << std::endl;
}
