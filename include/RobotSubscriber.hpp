#ifndef ROBOT_SUBSCRIBER_HPP
#define ROBOT_SUBSCRIBER_HPP

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/topic/Topic.hpp>

#include "SubListener.hpp"
#include "RobotTelemetry.hpp"
#include "RobotTelemetryPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;

class RobotSubscriber
{
public:
    RobotSubscriber();
    ~RobotSubscriber();

    bool init(DataReaderQos& qos);
    void printReaderQoS(const DataReaderQos& qos);
    void run();
    int getMatchedPublishers() const;
    void stop();
    uint32_t getTotalMessages() const;

private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    Topic* topic_;
    DataReader* reader_;
    TypeSupport type_;
    SubListener listener_;
};

#endif