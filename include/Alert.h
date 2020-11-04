//
//Alert.h
//

#ifndef ALERT_H
#define ALERT_H

#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/LocalDateTime.h"

struct Alert
{
    Alert(Ping& p,Node& n,Configuration& c)
    {
        sent = false;

        if(p.get_hostname().empty() && !n.name.empty()){

            hostname = Poco::toUpper(n.name);

        }else if(n.name.empty()){

            hostname = Poco::toUpper(n.address);

        }else{

            hostname = Poco::toUpper(p.get_hostname());
        }

        if(p.get_hostaddr().empty()){
            address = n.address;
        }else{
            address = p.get_hostaddr();
        }

        if(p.get_error().empty()){
            error = "Unknown error";
        }else{
            error = p.get_error();
        }

        subject = "Host " + hostname + " Unresponsive";

        time_stamp = Poco::DateTimeFormatter::format(Poco::LocalDateTime(),Poco::DateTimeFormat::RFC1123_FORMAT);
        
        std::ostringstream os;
       
        os << "This alert was generated at: " << time_stamp
            << "\n\nThe host " << hostname << " has failed to respond to pings for "
            << c.get_max_fail() * c.get_interval() << " seconds."
            << "\n\nThe last error generated was: " << error;

        message = os.str();
    }

    bool sent;
    std::string hostname;
    std::string address;
    std::string subject;
    std::string message;
    std::string error;
    std::string time_stamp;
};

#endif //ALERT_H
