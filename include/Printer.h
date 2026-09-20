#pragma once

class Printer
{
public:
    static void printTo(std::filesystem::path const& path, int interval);
    static void printToConsole();
};