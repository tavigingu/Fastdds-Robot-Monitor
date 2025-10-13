#include "RobotSubscriber.hpp"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <iostream>
#include <iomanip>

RobotSubscriber::RobotSubscriber()
    : participant_(nullptr)
    , subscriber_(nullptr)
    , topic_(nullptr)
    , reader_(nullptr)
    , type_(new RobotTelemetryPubSubType())
{
}

RobotSubscriber::~RobotSubscriber()
{
    stop();
}

void RobotSubscriber::init()
{
    std::cout << "[Subscriber] Initializing...\n" << std::endl;
    DomainParticipantQos pqos;

    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);

    if(participant_ == nullptr) {
        std::cerr << "[Subscriber] Error: Failed to create DomainParticipant" << std::endl;
        return false;
    }

    std::cout<<"[Subscriber] DomainParticipant created (Doamin 0)" << std::endl;

    //register data type RobotTelemetry
    type_.register_type(participant_);
    std::cout <<"[Subscriber] Data type registered" << std::endl;

    //create topic - same name as publisher
    topic_ = participant_->create_topic(
        "robot_telemetry",
        type.get_type_name(),
        TOPIC_QOS_DEFAULT
    );

    if(topic_ == nullptr) {
        std::cerr << "[Subscriber] Error: Failed to create Topic!" << std::endl;
        return false;
    }
    std::cout << "[Subscriber] Topic created: robot_telemetry" << std::endl;

    //create subscriber
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT);

    if(subscriber_ == nullptr) {
        std::cerr << "[Subscriber] Error: Failed to create Subscriber!" << std::endl;
        return false;
    }

    std::cout << "[Subscriber] Subscriber created" << std::endl;

    //create data reader
    reader_ = subscriber_->craete_datareader(
        topic_,
        DATAREADER_QOS_DEFAULT,
        &listener_);

    if(reader_ == nullptr) {
        std::cerr<<"[Subscriber] Error: Failed to create datareader" << std::endl;
        return false;
    }
    std::cout << "[Subscriber] Datareader created!" << std::endl;

    return true;
}

void RobotSubscriber::run()
{
    std::cout << "[Subscriber] Reading messages" std::endl;
    std::cout << "[Subscriber] Press enter to stop" std::endl;

    std::cin.ignore();
}

int RobotSubscriber::getMatchedPublishers() const
{
    return listener_.matched_;
}

uint32_t RobotSubscriber::getTotalMessages() const
{
    return listener_.samples_received_;
}

void RobotSUbscriber::stop()
{
    if (participant_ != nullptr)
    {
        // Șterge în ordine inversă creării
        if (subscriber_ != nullptr)
        {
            if (reader_ != nullptr)
            {
                subscriber_->delete_datareader(reader_);
                reader_ = nullptr;
            }
            participant_->delete_subscriber(subscriber_);
            subscriber_ = nullptr;
        }

        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
            topic_ = nullptr;
        }
    }

    DomainParticipantFactory::get_instance()->delete_participant(participant_);
    participant_ = nullptr;

    std::cout<<"[Subscriber] Stopped" std::endl;
}





