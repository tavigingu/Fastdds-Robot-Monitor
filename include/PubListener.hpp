#ifndef PUBLISTENER_HPP
#define PUBLISTENER_HPP

#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/core/status/PublicationMatchedStatus.hpp>

using namespace eprosima::fastdds::dds;

/**
 * @brief Listener for a DataWriter to track matched subscribers
 * 
 * This class listens to DataWriter events and keeps track of the
 * number of subscribers currently matched.
 */
class PubListener : public DataWriterListener
{
public:
    PubListener() : matched_(0) {}
    ~PubListener() override {}

    void on_publication_matched( DataWriter* writer,
        const PublicationMatchedStatus& info) override
    {
        if(info.current_count_change == 1)
        {
            matched_ = info.current_count;
            std::cout<<"[Publisher] Subscriber connected! Total: " << matched_ <<std::endl;
        }
        else if (info.current_count_change == -1)
        {
            matched_ = info.current_count;
            std::cout<<"[Publisher] Subscriber disconnected! Total: " << matched_ << std::endl;
        }
    }

    /// num of currently matched subscribers
    int matched_;
};
#endif