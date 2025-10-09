#ifndef ROBOT_PUBLISHER_HPP
#define ROBOT_PUBLISHER_HPP

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/Topic.hpp>

#include "RobotTelemetry.hpp"
#include "RobotTelemetryPubSubTypes.hpp"
#include "PubListener.hpp"

using namespace eprosima::fastdds::dds;

/**
 * @brief Publisher for robot telemetry
 * 
 * This class handles the publishing of telemetry data for a robot
 * It encapsulates all necessary DDS components (participant, publisher, topic and writer)
 */

 class RobotPublisher
 {
public:
    RobotPublisher();
    ~RobotPublisher();

    bool init();

    bool publish(RobotTelemetry& data);
    int getMatchedSubscribers() const;

    void stop();

private:
    //DDS components
    DomainParticipant* participant_;
    Publisher* publisher_;
    Topic* topic_;
    DataWriter* writer_;
    TypeSupport type_;
    PubListener listener_;
 };
#endif