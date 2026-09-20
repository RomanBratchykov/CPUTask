#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <atomic>

std::atomic<bool> running{true};


void my_handler(int s){
    const char message[] = "Caught";
    write(STDOUT_FILENO, message, strlen(message));
    running = false;
}

int main(int argc,char** argv)
{
    struct sigaction sig{};
    sig.sa_handler = my_handler;
    sig.sa_flagt = 0;
    sigemptyset(&sig.sa_mask);
    if (sigaction(SIGINT, &sig, NULL) == -1)
    {
        return 1;
    }
    while (running)
    {
        printf("i am printing shit");
    }
    return 0;
}