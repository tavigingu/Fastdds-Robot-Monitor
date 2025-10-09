#include "RobotPublisher.hpp"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <iostream>

RobotPublisher::RobotPublisher()
    :participant_(nullptr),
    publisher_(nullptr),
    topic_(nullptr),
    writer_(nullptr),
    type_(new RobotTelemetryPubSubType()) 
{
}

RobotPublisher::~RobotPublisher()
{
    stop();
}

bool RobotPublisher::init()
{
    std::cout << "[Publisher] Initializing ..." << std::endl;
    
    //create Domain participant
    DomainParticipantQos pqos;
    pqos.name("RobotPublisher_Participant");

    participant_ = DomainParticipantFactory::get_instance()->create_participant(
        0, // domain id
        pqos // qos settings
    );

    if(participant_ == nullptr){
        std::cerr<< "[Publisher] Error: Failed to create DomainParticipant!"<< std::endl;
        return false;
    }
    std::cout<<"[Publisher] DomainParticipant created (Domain 0)" << std::endl;

    //register data type
    type_.register_type(participant_);
    std::cout << "[Publisher] Datatype registered: " << type_.get_type_name();

    //create topic
    // topic = "robot_telemetry", type = RobotTelemetry
    topic_ = participant_->create_topic(
        "robot_telemetry",
        type_.get_type_name(),
        TOPIC_QOS_DEFAULT); //change it later

    if(topic_ == nullptr) {
        std::cerr << "[Publisher] Error: Failed to create Topic!" << std::endl;
        return false;
    }
    std::cout << "[Publisher] Topic created: robot_telemetry" << std::endl;

    //create publisher
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT);
    if (publisher_ == nullptr)
    {
        std::cerr << "[Publisher] Error: Failed to create Publisher!" << std::endl;
        return false;
    }
    std::cout << "[Publisher] Publisher created" << std::endl;

    //create writer
    writer_ = publisher_->create_datawriter(
        topic_,                      // topic to write
        DATAWRITER_QOS_DEFAULT,      // qos default
        &listener_);                 // listener for events 

    if (writer_ == nullptr)
    {
        std::cerr << "[Publisher] Error: Failed to create DataWriter!" << std::endl;
        return false;
    }
    std::cout << "[Publisher] DataWriter created" << std::endl;

    return true;
}   

bool RobotPublisher::publish(RobotTelemetry& data)
{
    if(writer_ == nullptr) {
        std::cerr << "[Publisher] Error: writer is not initialized!" << std::endl;
        return false;
    }

    //write data on topic
    //returns true for success
    if(writer_->write(&data)){
        return true;
    } else {
        std::cerr << "[Publisher] Error to public message!" << std:: endl;
        return false;
    }
}

//get num of subscribers
int RobotPublisher::getMatchedSubscribers() const
{
    return listener_.matched_;
}

void RobotPublisher::stop()
{
    if (participant_ != nullptr)
    {
        // Șterge în ordine inversă creării
        if (publisher_ != nullptr)
        {
            if (writer_ != nullptr)
            {
                publisher_->delete_datawriter(writer_);
                writer_ = nullptr;
            }
            participant_->delete_publisher(publisher_);
            publisher_ = nullptr;
        }

        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
            topic_ = nullptr;
        }

        DomainParticipantFactory::get_instance()->delete_participant(participant_);
        participant_ = nullptr;

        std::cout << "[Publisher] Oprit și curățat" << std::endl;
    }
}
