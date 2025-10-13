#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/topic/Topic.hpp>

#include "RobotTelemetry.hpp"
#include "RobotTelemetryPubSubTypes.hpp"

class SubListener : public DataReaderListener
{
public:
    SubListener() 
        : matched_(0)
        , samples_received_(0)
    {}
    
    ~SubListener() override {}

    on_subscription_matched(
    DataReader* reader,
    const SubscriptionMatchedStatus& info) override
    {
        if (info.current_count_change == 1)
        {
            matched_ = info.current_count;
            std::cout << "[Subscriber Listener] Publisher connected! Total: " << matched_ << std::endl;
        }
        else if (info.current_count_change == -1)
        {
            matched_ = info.current_count;
            std::cout << "[Subscriber Listener] Publisher connected! Total: " << matched_ << std::endl;
        }
    }

    on_data_available(DataReader* reader)
    {
        RobotTelemetry telemetry;
        SampleInfo info;

        ReturnCode_t ret = reader->take_next_sample(&telemetry, &info);

        if (ret == RETCODE_OK)
        {
            if (info.valid_data)
            {
                samples_received_++;

                std::cout << "\n[Subscriber] Mesaj #" << samples_received_ << " primit:" << std::endl;
                std::cout << "  Robot ID:    " << telemetry.id() << std::endl;
                std::cout << "  Poziție:     (" << std::fixed << std::setprecision(2) 
                        << telemetry.x() << ", " << telemetry.y() << ") m" << std::endl;
                std::cout << "  Orientare:   " << telemetry.orientation() << " rad" << std::endl;
                std::cout << "  Baterie:     " << telemetry.battery_level() << "%" << std::endl;
                std::cout << "  Viteză:      " << telemetry.speed() << " m/s" << std::endl;
                std::cout << "  Status:      " << telemetry.status() << std::endl;
                std::cout << "  Timestamp:   " << telemetry.timestamp() << " ns" << std::endl;
                std::cout << std::string(50, '-') << std::endl;
            }
        }
        else if (ret == RETCODE_NO_DATA)
        {
            std::cout << "[Subscriber] on_data_available apelat dar nu există date!" << std::endl;
        }
        else
        {
            std::cerr << "[Subscriber] Eroare la citirea datelor! ReturnCode: " << ret() << std::endl;
        }
    }

   

    int matched_;                // num of publishers connected
    uint32_t samples_received_;  // num of messages received

} 
