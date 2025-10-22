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

//bool RobotPublisher::init()
bool RobotPublisher::init(const DataWriterQos& qos)
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
        qos,      // qos custom
        &listener_);                 // listener for events 

    if (writer_ == nullptr)
    {
        std::cerr << "[Publisher] Error: Failed to create DataWriter!" << std::endl;
        return false;
    }

    printWriterQoS(qos);
    std::cout << "[Publisher] DataWriter created" << std::endl;

    return true;
}   

bool RobotPublisher::publish(RobotTelemetry& data)
{
    if (writer_ == nullptr)
    {
        std::cerr << "[Publisher] Error: writer is not initializated!" << std::endl;
        return false;
    }

    ReturnCode_t ret = writer_->write(&data);
    
    if (ret == RETCODE_OK)
    {
        return true;
    }
    else
    {
        std::cerr << "[Publisher] Error to public message! ReturnCode: " << ret << std::endl;
        
        // Debugging info
        if (ret == RETCODE_ERROR)
            std::cerr << "  -> RETCODE_ERROR: generic error" << std::endl;
        else if (ret == RETCODE_BAD_PARAMETER)
            std::cerr << "  -> RETCODE_BAD_PARAMETER: invalid parameter" << std::endl;
        else if (ret == RETCODE_TIMEOUT)
            std::cerr << "  -> RETCODE_TIMEOUT: Timeout" << std::endl;
        
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

void RobotPublisher::printWriterQoS(const DataWriterQos& qos)
{
    std::cout << "[Publisher] DataWriter creat cu următorul QoS:" << std::endl;
    
    std::cout << "  - Reliability: " 
              << (qos.reliability().kind == RELIABLE_RELIABILITY_QOS ? "RELIABLE" : "BEST_EFFORT") 
              << std::endl;
    
    std::cout << "  - Durability:  " 
              << (qos.durability().kind == TRANSIENT_LOCAL_DURABILITY_QOS ? "TRANSIENT_LOCAL" : "VOLATILE") 
              << std::endl;
    
    std::cout << "  - History:     " 
              << (qos.history().kind == KEEP_LAST_HISTORY_QOS ? "KEEP_LAST" : "KEEP_ALL");
    if (qos.history().kind == KEEP_LAST_HISTORY_QOS)
        std::cout << "(" << qos.history().depth << ")";
    std::cout << std::endl;
    
    if (qos.deadline().period.seconds > 0 || qos.deadline().period.nanosec > 0)
    {
        std::cout << "  - Deadline:    " << qos.deadline().period.seconds << "s " 
                  << qos.deadline().period.nanosec / 1000000 << "ms" << std::endl;
    }
    
    if (qos.lifespan().duration.seconds > 0 || qos.lifespan().duration.nanosec > 0)
    {
        std::cout << "  - Lifespan:    " << qos.lifespan().duration.seconds << "s" << std::endl;
    }
}