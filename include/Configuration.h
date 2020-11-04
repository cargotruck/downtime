//
//Configuration.h
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/String.h"

class Configuration
{
public:
    Configuration(std::string _email_host,
        std::string _email_sender,
        std::string _email_recipient,
        int _ping_interval,
        int _max_fail,
        std::string _log_dir)
        :
        email_host{_email_host},
        email_sender{_email_sender},
        email_recipient{_email_recipient},
        ping_interval{_ping_interval},
        max_fail{_max_fail}
    {
        //removes trailing '/'. Poco library will no accept it.
        while(true){
            if(_log_dir[_log_dir.size() - 1] == '/'){
                _log_dir = Poco::remove(_log_dir,'/',_log_dir.size() - 1);
            }else{  
                break;
            }
        }
        
        log_dir = _log_dir;
        if(!log_dir.isDirectory()){
            Poco::File ld{log_dir};
            ld.createDirectory();
        }
    }
   
    std::string get_host() {return email_host;}
    std::string get_sender() {return email_sender;}
    std::string get_recipient() {return email_recipient;}
    std::string get_log_loc() {return log_dir.toString();}
    int get_interval() {return ping_interval;}
    int get_max_fail() {return max_fail;} 

private:
    std::string email_host;
    std::string email_sender;
    std::string email_recipient;
    int ping_interval;
    int max_fail;
    Poco::Path log_dir;
};

#endif //CONFIGURATION_H
