#include "RobotSimulator.hpp"
#include <cmath>
#include <chrono>

RobotSimulator::RobotSimulator(const std::string& robot_id)
    : robot_id_(robot_id)
    , x_(0.0)
    , y_(0.0)
    , orientation_(0.0)
    , motion_mode_(MotionMode::STATIONARY)
    , velocity_(0.0)
    , angular_velocity_(0.2)
    , motion_angle_(0.0)
    , circle_radius_(5.0)
    , circle_center_x_(0.0)
    , circle_center_y_(0.0)
    , battery_level_(100.0f)
    , battery_drain_rate_(0.1f)    // 0.1% per sec
    , battery_charge_rate_(1.0f)   // 1% per sec
    , low_battery_threshold_(20.0f)
    , is_charging_(false)
    , time_(0.0)
{
}

// main update - called every tick
void RobotSimulator::update(double dt)
{
    // update motion
    switch (motion_mode_)
    {
        case MotionMode::CIRCULAR:
            updateCircularMotion(dt);
            break;
        
        case MotionMode::LINEAR:
            updateLinearMotion(dt);
            break;
        
        case MotionMode::STATIONARY:
            break;
    }

    updateBattery(dt);

    //increment total time
    time_ += dt;
}

void RobotSimulator::updateCircularMotion(double dt)
{    
    double angle = angular_velocity_ * time_;
    
    x_ = circle_center_x_ + circle_radius_ * std::cos(angle);
    y_ = circle_center_y_ + circle_radius_ * std::sin(angle);
    
    orientation_ = angle + M_PI / 2.0; 
    
    while (orientation_ > 2 * M_PI)
        orientation_ -= 2 * M_PI;
    while (orientation_ < 0)
        orientation_ += 2 * M_PI;
    
    velocity_ = circle_radius_ * std::abs(angular_velocity_);
}

void RobotSimulator::updateLinearMotion(double dt)
{

    x_ += velocity_ * std::cos(motion_angle_) * dt;
    y_ += velocity_ * std::sin(motion_angle_) * dt;
    
    orientation_ = motion_angle_;
}

void RobotSimulator::updateBattery(double dt)
{
    if (is_charging_)
    {
        //charging
        battery_level_ += battery_charge_rate_ * dt;
        
        if (battery_level_ >= 100.0f)
        {
            battery_level_ = 100.0f;
            is_charging_ = false; 
        }
    }
    else
    {
        //discharge only if robot is moving
        if (motion_mode_ != MotionMode::STATIONARY && velocity_ > 0.0)
        {
            battery_level_ -= battery_drain_rate_ * dt;
            
            if (battery_level_ <= 0.0f)
            {
                battery_level_ = 0.0f;
                is_charging_ = true; 
            }
        }
    }
}

std::string RobotSimulator::determineStatus() const
{
    // Priority: CHARGING > LOW_BATTERY > moving module
    
    if (is_charging_)
    {
        return "CHARGING";
    }
    
    if (battery_level_ <= low_battery_threshold_ && battery_level_ > 0.0f)
    {
        return "LOW_BATTERY";
    }
    
    if (battery_level_ <= 0.0f)
    {
        return "DISCHARGED";
    }
    
    switch (motion_mode_)
    {
        case MotionMode::CIRCULAR:
        case MotionMode::LINEAR:
            return (velocity_ > 0.0) ? "MOVING" : "IDLE";
        
        case MotionMode::STATIONARY:
            return "IDLE";
        
        default:
            return "UNKNOWN";
    }
}

RobotTelemetry RobotSimulator::generateTelemetry()
{
    RobotTelemetry telemetry;
    
    telemetry.id(robot_id_);
    telemetry.x(x_);
    telemetry.y(y_);
    telemetry.orientation(orientation_);
    telemetry.battery_level(battery_level_);
    telemetry.speed(velocity_);
    telemetry.status(determineStatus());
    
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    uint64_t timestamp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    telemetry.timestamp(timestamp_ns);
    
    return telemetry;
}

void RobotSimulator::setCircularMotion(double radius, double angular_velocity)
{
    motion_mode_ = MotionMode::CIRCULAR;
    circle_radius_ = radius;
    angular_velocity_ = angular_velocity;
    
    circle_center_x_ = x_;
    circle_center_y_ = y_;
}

void RobotSimulator::setLinearMotion(double velocity, double angle)
{
    motion_mode_ = MotionMode::LINEAR;
    velocity_ = velocity;
    motion_angle_ = angle;
}

void RobotSimulator::setStationary(double x, double y)
{
    motion_mode_ = MotionMode::STATIONARY;
    x_ = x;
    y_ = y;
    velocity_ = 0.0;
}

void RobotSimulator::setBatteryDrainRate(float rate)
{
    battery_drain_rate_ = rate;
}

void RobotSimulator::setBatteryChargeRate(float rate)
{
    battery_charge_rate_ = rate;
}

void RobotSimulator::setLowBatteryThreshold(float threshold)
{
    low_battery_threshold_ = threshold;
}

void RobotSimulator::setBatteryLevel(float level)
{
    battery_level_ = std::max(0.0f, std::min(100.0f, level));
    
    if (battery_level_ > 0.0f)
    {
        is_charging_ = false;
    }
}

void RobotSimulator::reset()
{
    x_ = 0.0;
    y_ = 0.0;
    orientation_ = 0.0;
    battery_level_ = 100.0f;
    is_charging_ = false;
    time_ = 0.0;
    velocity_ = 0.0;
    
}