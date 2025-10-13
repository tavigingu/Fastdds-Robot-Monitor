#ifndef ROBOT_SUBSCRIBER_HPP
#define ROBOT_SUBSCRIBER_HPP

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/topic/Topic.hpp>

#include "SubListener.hpp"
#include "RobotTelemetry.hpp"
#include "RobotTelemetryPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;#define ROBOT_SUBSCRIBER_HPP

class RobotSubscriber
{
public:
    RobotSubscriber();
    ~RobotSubscriber();

    bool init();
    void run();
    int getMatchedPublishers();
    void stop();
    uint32_t getTotalMessages();

private:
    DomainParticipant* participant_;
    Subsciber* subscriber_;
    Topic* topic
    DataReader* reader_;
    TypeSupport type_;
    SubListener listener_;
}