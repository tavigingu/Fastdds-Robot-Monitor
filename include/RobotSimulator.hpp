#ifndef ROBOT_SIMULATOR_HPP
#define ROBOT_SIMULATOR_HPP

#include "RobotTelemetry.hpp"
#include <string>

class RobotSimulator
{
public:
     enum class MotionMode
    {
        CIRCULAR,      // Mișcare circulară
        LINEAR,        // Mișcare liniară
        STATIONARY     // Robot static
    };

    explicit RobotSimulator(const std::string& robot_id);

    //update by one time step
    // dt = time in sec ( 0.1 for 100ms)
    void update(double dt);

    RobotTelemetry generateTelemetry();

    void setCircularMotion(double radius, double angular_velocity);
    void setLinearMotion(double velocity, double angle);
    void setStationary(double x, double y);
    void setBatteryDrainRate(float rate);
    void setBatteryChargeRate(float rate);
    void setLowBatteryThreshold(float threshold);
    void setBatteryLevel(float level);
    //reset to initial state
    void reset();

    double getSimulationTime() const { return time_; }

    MotionMode getMotionMode() const { return motion_mode_; }

private:
    std::string robot_id_;

    double x_;
    double y_;
    double orientation_;

    MotionMode motion_mode_;
    double velocity_;
    double angular_velocity_;
    double motion_angle_;

    double circle_radius_;
    double circle_center_x_;
    double circle_center_y_;

    float battery_level_;
    float battery_drain_rate_;
    float battery_charge_rate_;
    float low_battery_threshold_;
    bool is_charging_;

    double time_;

    void updateCircularMotion(double dt);
    void updateLinearMotion(double dt);
    void updateBattery(double dt);
    std::string determineStatus() const;


};

#endif 