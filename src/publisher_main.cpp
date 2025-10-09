#include "RobotPublisher.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <signal.h>

//graceful shutdown
volatile sig_atomic g_running = 1;

void signalHandler(int signum)
{
    std::cout<<"[Publisher main] Signal received (" << signum << "), stopping..." << stdl::endl;
    g_running = 0;
}

int main()
{
    std::cout<< "=== Robot Telemetry Publisher ==="<<std::endl;
    std::cout<< " Ctrl _ C to stop\n"<<stdl::endl;

    //handler ctrl+c
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    RobotPublisher publisher;

    if(!publisher.init())
    {
        std::cerr << "[Publisher main] Init error" << std::endl;
        return -1;
    }

    std::cout << "[Publihser main] Waitin subscribers ... " << std::endl;


    //wait subscribers
    std::this_thread::sleep_for(std::chrono::seconds(2));

    //TO DO: implement a robot simulator to generate data
}