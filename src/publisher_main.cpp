#include "RobotPublisher.hpp"
#include "RobotSimulator.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <signal.h>

//graceful shutdown
volatile sig_atomic_t g_running = 1;

void signalHandler(int signum)
{
    std::cout<<"[Publisher main] Signal received (" << signum << "), stopping..." << std::endl;
    g_running = 0;
}

RobotSimulator createDefaultSimulator(const std::string& robot_id)
{
    RobotSimulator simulator(robot_id);
    simulator.setCircularMotion(5.0, 0.2);
    simulator.setBatteryDrainRate(0.1f);
    simulator.setBatteryChargeRate(1.0f);
    simulator.setLowBatteryThreshold(20.0f);
    return simulator;
}

void printTelemetryInfo(
    const RobotTelemetry& telemetry,
    int message_count,
    int subscribers)
{
    std::cout << "[Main] Msg #" << std::setw(4) << message_count
              << " | Pos: (" << std::fixed << std::setprecision(2)
              << std::setw(6) << telemetry.x()
              << ", " << std::setw(6) << telemetry.y() << ")"
              << " | θ: " << std::setw(5) << telemetry.orientation()
              << " | Battery: " << std::setw(5) << telemetry.battery_level() << "%"
              << " | Speed: " << std::setw(4) << telemetry.speed() << " m/s"
              << " | Status: " << std::setw(12) << std::left << telemetry.status() << std::right
              << " | Subs: " << subscribers
              << std::endl;
}


int main()
{
    std::cout<< "=== Robot Telemetry Publisher ==="<<std::endl;
    std::cout<< " Ctrl _ C to stop\n"<<std::endl;

    //handler ctrl+c
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    RobotPublisher publisher;

    if(!publisher.init())
    {
        std::cerr << "[Publisher main] Init error" << std::endl;
        return -1;
    }

    //DONE: implement a robot simulator to generate data
    auto simulator = createDefaultSimulator("robo003");

    std::cout << "[Publihser main] Waitin subscribers ... " << std::endl;

    //wait subscribers
    std::this_thread::sleep_for(std::chrono::seconds(2));

    const double dt = 0.1;
    int message_count = 0;

    std::cout<< "[Publisher main] Start publishing" << std::endl;

    while(g_running)
    {
        simulator.update(dt);
        RobotTelemetry telemetry = simulator.generateTelemetry();

        //public data
        if(publisher.publish(telemetry))
        {
            message_count++;

            //info every 10 messages
            if (message_count % 10 == 0)
                printTelemetryInfo(telemetry, message_count, publisher.getMatchedSubscribers());
            
            if(simulator.getSimulationTime() > 20)
            {
                std::cout<<"[Main publisher] reset simulation" << std::endl;
                simulator.reset();
                simulator.setCircularMotion(5, 0.2); // reconfig motion
            } 

        }
        else 
        {
            std::cerr << "[Main publisher] Error to public!" << std::endl;
        }

        // wait for the next tick
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    double total_sim_time = simulator.getSimulationTime();

    std::cout << "\n[Main] Total messages published: " << message_count << std::endl;
    std::cout << "[Main] Total simulated time: " << std::fixed << std::setprecision(1)
          << total_sim_time << " seconds" << std::endl;


    publisher.stop();
    std::cout <<"[Main publisher] Publisher stopped!" << std::endl;
}