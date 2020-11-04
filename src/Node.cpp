//
// Node.cpp
//

#include <string>
#include <sstream>

#include "../include/Node.h"

Node::Node(std::string _name,std::string _address):
    name{_name},
    address{_address},
    last_result{0},
    failed_resp{0},
    alert_sent{false}
{
    const char sep = '.';
    const std::string bad_flag = "-1";
    int oct1,oct2,oct3,oct4;
    char sep1,sep2,sep3;
    std::stringstream ss{address};

    if(ss.str().empty()) {return;}

    if(ss >> oct1 >> sep1 >> oct2 >> sep2 >> oct3 >> sep3 >> oct4){
        if(sep1 != sep || sep2 != sep || sep3 != sep) {address = bad_flag; return;}
        if(oct1 < 0 || oct1 > 255) {address = bad_flag; return;}
        if(oct2 < 0 || oct2 > 255) {address = bad_flag; return;}
        if(oct3 < 0 || oct3 > 255) {address = bad_flag; return;}
        if(oct4 < 0 || oct4 > 255) {address = bad_flag; return;}
    }else{
        address = bad_flag;
        return;
    }
}
