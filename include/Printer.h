#pragma once

#include "Parser.h"

#include <filesystem>
#include <signal.h>
#include <vector>
#include <string>


class Printer
{
public:
    static void printToFile(std::filesystem::path const& path,int interval, std::vector<CpuCore>& cpu_cores, volatile sig_atomic_t const& running);

    static void printToFile(std::filesystem::path const& path, std::vector<CpuCore> const& cpu_cores, std::string const& time);

    static void printToFile(std::filesystem::path const& path, std::vector<CpuCore> const& cpu_cores, std::string const& time, int core);

    static void printToConsole(std::vector<CpuCore> const& cpu_cores, std::string const& time);

    static void printToConsoleSpecific(std::vector<CpuCore> const& cpu_cores, std::string const& time, int core);
};
