#ifndef QOS_PROFILES_HPP
#define QOS_PROFILES_HPP

#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/core/policy/QosPolicies.hpp>

class QoSProfiles 
{
public:
    static DataWriterQos getReliableTransientWriterQoS()
    {
        DataWriterQOs qos;

        //reliability: guarantees delivery 
        qos.reliability().kind = RELIABLE_RELIABILITY_QOS;

        //durability: keeps history for new subscribers
        qos.durability().kind = TRANSIENT_LOCAL_DURABILITY

        qos.history().kind = KEEP_LAST_HISTORY_QOS;
        qos.history().depth =  10; 

        return qos;
    }

    static DataReaderQos getReliableTransientReaderQoS()
    {
        DataReaderQos qos;

        //it must match the writer!
        qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.history().kind = KEEP_LAST_HISTORY_QOS;
        qos.history().depth = 10;
    }

    static DataReaderQos getBestEffortReaderQos()
    {
        DataReader qos;

        qos.realiability().kind = BEST_EFFORT_RELIABILITY_QOS;
        qos.realiability().kind = VOLATILE_DURABILITY_QOS;
        qos.history().kind = KEEP_LAST_HISTORY_QOS;
        qos.history().depth = 1;

        return qos;
    }

     static DataWriterQos getReliableWithDeadlineWriterQoS()
    {
        DataWriterQos qos;

        qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.history().kind = KEEP_LAST_HISTORY_QOS;
        qos.history().depth = 10;

        // DEADLINE: must send at least one message every 500ms
        qos.deadline().period = eprosima::fastdds::dds::Duration_t(0, 500000000); // 500ms

        return qos;
    }

    static DataReaderQos getReliableWithDeadlineReaderQoS()
    {
        DataReaderQos qos;

        qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.history().kind = KEEP_LAST_HISTORY_QOS;
        qos.history().depth = 10;

        // DEADLINE: except a message every 500ms, otherwise trigger the callback
        qos.deadline().period = eprosima::fastdds::dds::Duration_t(0, 500000000); // 500ms

        return qos;
    }

    static DataWriterQos getReliableWithLifespanWriterQoS()
    {
        DataWriterQos qos;

        qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.history().kind = KEEP_LAST_HISTORY_QOS;
        qos.history().depth = 10;

        // LIFESPAN: messages expiers after 5 minutes
        qos.lifespan().duration = eprosima::fastdds::dds::Duration_t(5, 0); // 5 sec

        return qos;
    }

    static DataReaderQos getReliableWithLifespanReaderQoS()
    {
        DataReaderQos qos;

        qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.history().kind = KEEP_LAST_HISTORY_QOS;
        qos.history().depth = 10;

        // reader doesnt set lifespan ( writer policy)
        // but it will automatically ignore expired messages

        return qos;
    }

    static DataWriterQos getReliableKeepAllWriterQoS()
    {
        DataWriterQos qos;

        qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        
        // KEEP_ALL: keep all messages (not just the last N)
        qos.history().kind = KEEP_ALL_HISTORY_QOS;
        
        qos.resource_limits().max_samples = 1000;  // Max 1000 messages in buffer

        return qos;
    }

    static DataReaderQos getReliableKeepAllReaderQoS()
    {
        DataReaderQos qos;

        qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.history().kind = KEEP_ALL_HISTORY_QOS;
        qos.resource_limits().max_samples = 1000;

        return qos;
    }

    static void printQoSInfo(const DataWriterQos& qos, const std::string& name = "Writer")
    {
        std::cout << "\n=== QoS Profile: " << name << " ===" << std::endl;
        std::cout << "Reliability: " 
                  << (qos.reliability().kind == RELIABLE_RELIABILITY_QOS ? "RELIABLE" : "BEST_EFFORT") 
                  << std::endl;
        std::cout << "Durability:  " 
                  << (qos.durability().kind == TRANSIENT_LOCAL_DURABILITY_QOS ? "TRANSIENT_LOCAL" : "VOLATILE") 
                  << std::endl;
        std::cout << "History:     " 
                  << (qos.history().kind == KEEP_LAST_HISTORY_QOS ? "KEEP_LAST" : "KEEP_ALL");
        if (qos.history().kind == KEEP_LAST_HISTORY_QOS)
            std::cout << "(" << qos.history().depth << ")";
        std::cout << std::endl;
        
        if (qos.deadline().period.seconds > 0 || qos.deadline().period.nanosec > 0)
        {
            std::cout << "Deadline:    " << qos.deadline().period.seconds << "s " 
                      << qos.deadline().period.nanosec / 1000000 << "ms" << std::endl;
        }
        
        if (qos.lifespan().duration.seconds > 0 || qos.lifespan().duration.nanosec > 0)
        {
            std::cout << "Lifespan:    " << qos.lifespan().duration.seconds << "s" << std::endl;
        }
        
        std::cout << "================================\n" << std::endl;
    }

    static void printQoSInfo(const DataReaderQos& qos, const std::string& name = "Reader")
    {
        std::cout << "\n=== QoS Profile: " << name << " ===" << std::endl;
        std::cout << "Reliability: " 
                  << (qos.reliability().kind == RELIABLE_RELIABILITY_QOS ? "RELIABLE" : "BEST_EFFORT") 
                  << std::endl;
        std::cout << "Durability:  " 
                  << (qos.durability().kind == TRANSIENT_LOCAL_DURABILITY_QOS ? "TRANSIENT_LOCAL" : "VOLATILE") 
                  << std::endl;
        std::cout << "History:     " 
                  << (qos.history().kind == KEEP_LAST_HISTORY_QOS ? "KEEP_LAST" : "KEEP_ALL");
        if (qos.history().kind == KEEP_LAST_HISTORY_QOS)
            std::cout << "(" << qos.history().depth << ")";
        std::cout << std::endl;
        
        if (qos.deadline().period.seconds > 0 || qos.deadline().period.nanosec > 0)
        {
            std::cout << "Deadline:    " << qos.deadline().period.seconds << "s " 
                      << qos.deadline().period.nanosec / 1000000 << "ms" << std::endl;
        }
        
        std::cout << "================================\n" << std::endl;
    }

}

#endif // QOS_PROFILES_HPP