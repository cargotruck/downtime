//
// Cmd_main.cpp
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/LocalDateTime.h"
#include "Poco/String.h"

#include "../include/Configuration.h"
#include "../include/mailer.h"
#include "../include/Node.h"
#include "../include/Ping.h"
#include "../include/Utilities.h"
#include "../include/Alert.h"

//-------------------------------------------------------------------------

Configuration read_config_file();

//GLOBAL CONSTANTS
Configuration config{dtUtil::load_config_file()};

//-------------------------------------------------------------------------

bool send_alert(Alert alert)
{
    Email email{config.get_host(),config.get_sender(),config.get_recipient()};
    std::cout << "SENDING ALERT TO NETWORK ADMIN" << std::endl;

    try{
        dtUtil::enter_log("An alert was send via " 
            + config.get_host() 
            + "."
            + " ALERT: " 
            + alert.message);
        email.set_subject(alert.subject);
        email.set_content(alert.message);
        email.send();
        return true;

    }catch(Poco::Exception& exc){
        dtUtil::enter_log("An alert failed to send via " 
            + config.get_host() 
            + "."
            + " ERROR: " + exc.displayText() 
            + "."
            + " ALERT: " + alert.message);

        std::cerr << "There was a problem sending the message: " << exc.displayText() << std::endl;
        return false;
    }
    
}

//-------------------------------------------------------------------------

void process_alerts(std::vector<Alert>& alerts)
{
    for(int i = 0;i < alerts.size();++i){
        if(alerts[i].sent == false) alerts[i].sent = send_alert(alerts[i]);
    }    

    std::vector<Alert> failed_to_send;
    for(int i = 0;i < alerts.size();++i){
        if(alerts[i].sent == false) failed_to_send.push_back(alerts[i]);
    }

    alerts = failed_to_send;
}

//-------------------------------------------------------------------------

void main_loop(std::vector<Node>& nodes) 
{
    std::vector<Alert> alerts;
    
    while(true){
        int responders = 0;
        for(int i = 0;i < nodes.size();++i){
            std::string target = "";
            try{
                if(nodes[i].address.empty() || nodes[i].address == dtUtil::BAD_ADDRESS_FLAG){
                    target = nodes[i].name;
                }else{
                    target = nodes[i].address;
                }

                Ping ping{target};
                int echo = ping.send_ping();

                if(echo != 1){
                    nodes[i].failed_resp += 1;
                    dtUtil::enter_log("Node " + nodes[i].address + "(" + nodes[i].name + ")"
                                + " failed to respond to ping attempt."
                                + " Node " + nodes[i].address + " (" + nodes[i].name + ")"
                                + " has failed to respond " + std::to_string(nodes[i].failed_resp) + " times.");
                }else{
                    nodes[i].failed_resp = 0;
                    nodes[i].alert_sent = false;
                    ++responders;
                }

                if(nodes[i].failed_resp >= config.get_max_fail() && nodes[i].alert_sent == false){
                    dtUtil::enter_log("Node " + nodes[i].address + " (" + nodes[i].name + ")"
                        + " met failure to respond threshold. An alert has been generatd.");
                    alerts.push_back({ping,nodes[i],config});
                    nodes[i].alert_sent = true;
                }

                process_alerts(alerts);

            }catch(Poco::Exception& exc){
                dtUtil::enter_log("Ping attempt failed. ERROR: " + exc.displayText());
                std::cerr << "\n\n\tERROR: " << exc.displayText() << std::endl; 
            }
        }

        dtUtil::enter_log("Pings sent. " + std::to_string(responders) + " of " 
            + std::to_string(nodes.size()) + " nodes responded.");
        dtUtil::sleep(config.get_interval());
    }
}

//-------------------------------------------------------------------------

int main()
{
    std::vector<Node> nodes;
    nodes = dtUtil::read_address_book();
    main_loop(nodes);
}
