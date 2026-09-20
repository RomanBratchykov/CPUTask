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
#include <time.h>

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
                    cores = Parser::parseAll();
                    printf("Parsed %d cores\n", static_cast<int>(cores.size()));
                    timestr.resize(32);
                    auto now = std::chrono::system_clock::now() + std::chrono::hours(3);
                    auto ts = std::chrono::system_clock::to_time_t(now);
                    strftime(timestr.data(), timestr.size(), "%H:%M:%S", std::localtime(&ts));
                    printf("Current time: %s", std::ctime(&ts));
                    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
                    state = State::Run;
                    printf("Welcome to the CPU Monitor \n");
                    printf("Enter desired proccess:\n1:Show CPU usage for now.\n2:Start writing info to console for each core and selected time.\n3:Save info to txt file for all cores with selected interval\n4:Save info to txt file for selected core with selected interval\n");

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
                        {
                            printf("Showing CPU usage for now\n");
                            Parser::updateAll(cores);
                            auto now = std::chrono::system_clock::now() + std::chrono::hours(3);
                            auto ts = std::chrono::system_clock::to_time_t(now);
                            strftime(timestr.data(), timestr.size(), "%H:%M:%S", std::localtime(&ts));
                            Printer::printToConsole(cores, timestr);
                            Printer::printToFile(cores, timestr);
                            break;
                        }
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
                            auto now = std::chrono::system_clock::now() + std::chrono::hours(3);
                            auto ts = std::chrono::system_clock::to_time_t(now);
                            strftime(timestr.data(), timestr.size(), "%H:%M:%S", std::localtime(&ts));
                            Parser::updateAll(cores);
                            Printer::printToConsoleSpecific(cores, timestr, core);
                            Printer::printToFile(cores, timestr, core);
                        }
                        break;
                        case 3:
                        {
                            printf("Enter interval in milliseconds, Ctrl+C to stop: \n");
                            int interval = 0;
                            scanf("%d", &interval);
                            if (interval <= 0)
                            {
                                printf("Error, interval must be greater than zero: \n");
                                break;
                            }
                            Parser::updateAll(cores);
                            Printer::printToFile(interval, cores, running);
                        }
                            break;
                        case 4:
                        {
                            printf("Enter core number: \n");
                            int core = 0;
                            scanf("%d", &core);
                            if (core < 0 || static_cast<size_t>(core) >= cores.size())
                            {
                                printf("Error, you are not in core range: \n");
                                break;
                            }
                            printf("Enter interval in milliseconds, Ctrl+C to stop: \n");
                            int interval = 0;
                            scanf("%d", &interval);
                            if (interval <= 0)
                            {
                                printf("Error, interval must be greater than zero: \n");
                                break;
                            }
                            printf("Info about core %d\n", core);
                            Parser::updateAll(cores);
                            Printer::printToFile(interval, cores, running, core);
                        }
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
