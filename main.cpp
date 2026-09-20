#include <Printer.h>
#include <Calculator.h>
#include <Parser.h>

#include <signal.h>
#include <stdio.h>
#include <atomic>
#include <cstring>
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "include/Parser.h"


enum class State
{
    Init,
    Run,
    Stop
};

volatile sig_atomic_t running = 1;

void my_handler(int){
    const char message[] = "\nGently stopping\n";
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    running = 0;
}

int main()
{
    State state = State::Init;
    struct sigaction sig{};
    sig.sa_handler = my_handler;
    sig.sa_flags = 0;
    sigemptyset(&sig.sa_mask);
    if (sigaction(SIGINT, &sig, nullptr) == -1)
    {
        perror("sigaction");
        return 1;
    }
    std::vector<CpuCore> cores;
    std::string timestr;
    std::string filepath = "";

    while (running)
    {
        switch(state)
        {
            case State::Init:
            {
                try
                {
                    printf("Initializing\nReading file..\n");
                    auto cores = Parser::parseAll();
                    printf("Parsed %d cores\n", static_cast<int>(cores.size()));
                    auto now = std::chrono::system_clock::now();
                    auto time = std::chrono::system_clock::to_time_t(now);
                    std::tm localTime = *std::localtime(&time);

                    std::ostringstream oss;
                    oss << std::put_time(&localTime, "%H:%M:%S");

                    timestr = oss.str();
                    printf("Current time: %s", std::ctime(&time));
                    printf("enter filepath for saving your info:\n");
                    std::cin >> filepath;
                    state = State::Run;
                    printf("Welcome to the CPU Monitor \n");
                    printf("Enter desired proccess:\n1:Show CPU usage for now.\n2:Start writing info to console for each core and selected time.\n3:Save info to txt file for all cores with selected interval\n");

                }
                catch(std::exception& e)
                {
                    printf("Error occurred: %s\n", e.what());
                }
            }
                break;
            case State::Run:
            {
                try
                {
                    printf("Select task:\n");
                    int menu = 0;
                    scanf("%d", &menu);
                    switch (menu)
                    {
                        case 1:
                            printf("Showing CPU usage for now\n");
                            Parser::updateAll(cores);
                            Printer::printToConsole(cores, timestr);
                            Printer::printToFile(filepath, cores, timestr);
                            break;
                        case 2:
                        {
                            printf("Enter core number: \n");
                            int core = 0;
                            scanf("%d", &core);
                            if (core < 0 || static_cast<size_t>(core) >= cores.size())
                            {
                                printf("Error, you are not in core range: \n");
                                break;
                            }
                            printf("Info about core %d\n", core);
                            Parser::updateAll(cores);
                            Printer::printToConsoleSpecific(cores, timestr, core);
                            Printer::printToFile(filepath, cores, timestr, core);
                        }
                        break;
                        case 3:
                        {
                            printf("Enter interval in milliseconds, Ctrl+C to stop: \n");
                            int interval = 0;
                            scanf("%d", &interval);
                            Parser::updateAll(cores);
                            Printer::printToFile(filepath, interval, cores, running);
                        }
                            break;
                        default:
                            printf("Invalid option, please try again.\n");
                            break;
                    }
                }
                catch(std::exception& e)
                {
                    printf("Error occurred: %s\n", e.what());
                }
            }

                break;
            case State::Stop:
            {
                 running = false;
            }
                break;
        }
    }
    return 0;
}
