//
// Node.h
//

#ifndef NODE_H
#define NODE_H

struct Node
{
    Node(std::string _name,std::string _address);

    std::string name;
    std::string address;
    int last_result;
    int failed_resp;
    bool alert_sent;
};

#endif //NODE_H
