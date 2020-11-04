//
// Ping.h
//

#ifndef PING_H
#define PING_H

#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPClient.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/ICMPEventArgs.h"
#include "Poco/Delegate.h"

class Ping
{
public:
    Ping(std::string _target): 
        icmpClient(Poco::Net::IPAddress::IPv4),
        repetitions(1), 
        target(_target)
        {
            icmpClient.pingReply += Poco::delegate(this,&Ping::onReply);
            icmpClient.pingError += Poco::delegate(this,&Ping::onError);
        }

    void onReply(const void* pSender,Poco::Net::ICMPEventArgs& args);
    void onError(const void* pSender,Poco::Net::ICMPEventArgs& args);

    int send_ping();

    std::string get_error();
    std::string get_hostaddr();
    std::string get_hostname();
    int get_lag();
        
private:
    Poco::Net::ICMPClient icmpClient;
    int repetitions;
    std::string target;

    //info about ping:
    std::string hostname;
    std::string host_address;
    std::string error;
    int reply_time; //in milliseconds
};

#endif //PING_H
