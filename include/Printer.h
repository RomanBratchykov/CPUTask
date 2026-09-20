#pragma once

#include "Parser.h"

#include <filesystem>
#include <vector>
#include <string>


class Printer
{
public:
    static void printToFile(std::filesystem::path const& path, int interval);
    static void printToConsole(std::vector<CpuCore> const& cpu_cores, std::string const& time);
    static void printToConsoleSpecific(std::vector<CpuCore> const& cpu_cores, std::string const& time, int core);
};