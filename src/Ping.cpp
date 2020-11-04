//
// Ping.cpp
//

#include "../include/Ping.h"

//-------------------------------------------------------------------------

std::string Ping::get_error()
{

    return error;
}

//-------------------------------------------------------------------------

std::string Ping::get_hostaddr()
{
    return host_address;
}

//-------------------------------------------------------------------------

std::string Ping::get_hostname()
{
    return hostname;
}

//-------------------------------------------------------------------------

int Ping::get_lag()
{
    return reply_time;
}

//-------------------------------------------------------------------------

void Ping::onReply(const void* pSender,Poco::Net::ICMPEventArgs& args)
{
    hostname = args.hostName();
    host_address = args.hostAddress();
    reply_time = args.replyTime();
}

//-------------------------------------------------------------------------

void Ping::onError(const void* pSender,Poco::Net::ICMPEventArgs& args)
{
    error = args.error();
}

//-------------------------------------------------------------------------

int Ping::send_ping()
{
    return icmpClient.ping(target,repetitions);
}
