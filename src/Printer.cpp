#include "Printer.h"
#include "Calculator.h"

#include "filesystem"
#include <iostream>


class Printer
{
public:
    static void printTo(std::filesystem::path const& path, int interval)
    {

    }
    static void printToConsole()
    {
        std::cout << "Printing to console..." << std::endl;

    }
};