//
// Log.h
//

#ifndef LOG_H
#define LOG_H

#include <string>


struct Log
{
    Log(std::string _time,std::string _entry);

    std::string time;
    std::string entry;
};

#endif //LOG_H
