#include "RobotSubscriber.hpp"
#include <iostream>
#include <signal.h>

volatile sig_atommic_t g_running = 1;

void signalHandler(int signum)
{
    std::cout<<"[Main subscriber] Signal received )" << signum << "), stopping .. " << std::endl;
    g_running = 0;
}

int main()
{
    std::cout << "=== Robot Telemetry Subscriber ===" << std::endl;
    std::cout  << "Press ctrl+c or enter to stop" << std::endl;

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    RobotSubscriber subscriber;

    if(!subscriber.init())
    {   
        std::cerr << "[Main subscriber] init error"<<std::endl;
        return 1;
    }
    
    std::cout<<"[Main subscriber] Waiting publishers.." <<std::endl;
    std::cout << "[Main subscriber] messages will apear here" << std::endl;
    
    subscriber.run();
    std::cout << "[Main subscriber] Statistics: " std::endl;
    std::cout << "Total messages received: " << subscriber.getTotalMessages() <<std::endl;
    std::cout << "Publishers connected: " << subscriber.getMatchedPublishers() << std::endl;

    subscriber.stop();

    std::cout << "[Main] Done!" << std::endl;

    return 0;
}