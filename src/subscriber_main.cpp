#include "RobotSubscriber.hpp"
#include "QoSProfiles.hpp"
#include <iostream>
#include <signal.h>

volatile sig_atomic_t g_running = 1;

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

    std::cout << "[Main] Select a QoS profile:" << std::endl;
    std::cout << "  1. RELIABLE + TRANSIENT_LOCAL (Receive historical messages too!)" << std::endl;
    std::cout << "  2. BEST_EFFORT (Fast, no guarantees)" << std::endl;
    std::cout << "  3. RELIABLE + DEADLINE (Timeout detection)" << std::endl;
    std::cout << "  4. DEFAULT (No custom QoS)" << std::endl;
    std::cout << "Option [1-4]: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    DataReaderQos qos;
    switch(choice)
    {
        case 1:
            qos = QoSProfiles::getReliableTransientReaderQoS();
            std::cout << "\n[Main subscriber] Using: RELIABLE + TRANSIENT_LOCAL + KEEP_LAST(10)" << std::endl;
            std::cout << "[Main subscriber] NOTE: You will receive the last 10 historical messages upon connecting!" << std::endl;
            break;
        case 2:
            qos = QoSProfiles::getBestEffortReaderQoS();
            std::cout << "\n[Main subscriber] Using: BEST_EFFORT + VOLATILE" << std::endl;
            break;
        case 3:
            qos = QoSProfiles::getReliableWithDeadlineReaderQoS();
            std::cout << "\n[Main subscriber] Using: RELIABLE + DEADLINE(500ms)" << std::endl;
            std::cout << "[Main subscriber] NOTE: You will get an alert if the publisher does not send a message within 500ms!" << std::endl;
            break;
        case 4:
        default:
            qos = DATAREADER_QOS_DEFAULT;
            std::cout << "\n[Main subscriber] Using: DEFAULT QoS" << std::endl;
            break;
    }


    if(!subscriber.init(qos))
    {   
        std::cerr << "[Main subscriber] init error"<<std::endl;
        return 1;
    }
    
    std::cout<<"[Main subscriber] Waiting publishers.." <<std::endl;
    std::cout << "[Main subscriber] messages will apear here" << std::endl;
    
    subscriber.run();
    std::cout << "[Main subscriber] Statistics: " << std::endl;
    std::cout << "Total messages received: " << subscriber.getTotalMessages() <<std::endl;
    std::cout << "Publishers connected: " << subscriber.getMatchedPublishers() << std::endl;

    subscriber.stop();

    std::cout << "[Main] Done!" << std::endl;

    return 0;
}