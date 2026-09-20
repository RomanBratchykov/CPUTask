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
#include <ostream>


enum class State
{
    Init,
    Run,
    Stop
};

std::atomic<bool> running{true};

void my_handler(int s){
    const char message[] = "\nGently stopping\n";
    write(STDOUT_FILENO, message, strlen(message));
    running = false;
}

int main(int argc,char** argv)
{
    struct sigaction sig{};
    sig.sa_handler = my_handler;
    sig.sa_flags = 0;
    sigemptyset(&sig.sa_mask);
    std::vector<CpuCore> cores;
    std::string timestr;
    State state = State::Init;
    while (running)
    {
        switch(state)
        {
            case State::Init:
            {
                try
                {
                    printf("Initializing\nReading file..\n");
                    auto cores_first = Parser::parseAll();
                    printf("Conting load..\n");
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    auto cores_second = Parser::parseAll();
                    printf("Creating load data..\n");
                    cores = Calculator::acceptLoad(cores_first, cores_second);
                    printf("Parsed %d cores\n", static_cast<int>(cores.size()));
                    auto now = std::chrono::system_clock::now();
                    auto time = std::chrono::system_clock::to_time_t(now);
                    std::tm localTime = *std::localtime(&time);

                    std::ostringstream oss;
                    oss << std::put_time(&localTime, "%H:%M:%S");

                    timestr = oss.str();
                    printf("Current time: %s", std::ctime(&time));
                    state = State::Run;
                    printf("Welcome to the CPU Monitor \n");
                    printf("Enter desired proccess:\n1:Show CPU usage for now.\n2:Start writing info to console for each core and selected time.\n3:Save info to txt file for selected core and selected time\n4:Enter realtime monitoring and write to file.\n5:Enter realtime monitoring and write to file for specific core and specific time.\n");

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
                            Printer::printToConsole(cores, timestr);
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
                            Printer::printToConsoleSpecific(cores, timestr, core);
                        }
                        break;
                        case 3:
                            printf("Entering realtime monitoring\n");
                            break;
                        case 4:
                            printf("Entering realtime monitoring and writing to file\n");
                            break;
                        default:
                            printf("Invalid option\n");
                            break;
                    }
                    if (sigaction(SIGINT, &sig, NULL) == -1)
                    {
                        state = State::Stop;
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