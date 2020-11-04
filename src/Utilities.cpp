//
// Utilities.cpp
//

#include "Poco/String.h"

#include "../include/Utilities.h"

//Utilities for addressbook
//-------------------------------------------------------------------------

std::vector<Node> dtUtil::read_address_book(std::string address_book)
{
    const char sep = ':';
    std::vector<Node> n;

    std::ifstream ifs{address_book};
    if(!ifs){
        std::cerr << "\n\n\tERROR: Cannot open file or file does not exist: " << address_book << "."
                    << "\n\tThere are no network nodes to monitor. Terminating program.\n\n";
        std::exit(1);
    }

    std::string line;
    while(std::getline(ifs,line) && !line.empty()){
        std::vector<std::string> entries;
        std::string entry;
        std::istringstream is{line};
        for(char ch;is.get(ch);){
            if(ch != sep){
                entry += ch;
            }else{
                entries.push_back(Poco::trim(entry));
                entry = "";
            }
        }
        entries.push_back(Poco::trim(entry)); 

        //Control for valid address book entries (eg. FQDN : IPv4).
        Node test_node{entries[0],entries[1]};
        if(test_node.address == BAD_ADDRESS_FLAG){
            if(test_node.name.empty()){
                std::stringstream ss; 
                ss << "\n\n\tERROR while reading address book:"
                    << " Node does not have FQDN or valid IPv4 address."
                    << " \n\tNode was entered as: " << entries[0] << ':' << entries[1]
                    << " \n\tDiscarding node. Node will not be pinged." << std::endl;
                std::cerr << ss.str();
                enter_log(ss.str());
            }
        }else{
            if(test_node.address.empty()){
                std::stringstream ss;
                ss << "\n\n\tWARNING: Invalid IPv4 address: " << entries[1] 
                        << " detected for node " << to_upper(test_node.name) << "."
                        <<  "\n\tWill try pinging FQDN of node.";
                std::cerr << ss.str();
                enter_log(ss.str());
                n.push_back(Node{entries[0],entries[1]});
            }else{
                n.push_back(Node{entries[0],entries[1]});
            }
        }
        entry = "";
        entries = {};
    }

    if(n.size() == 0){
        std::cerr << "\n\n\tERROR: No nodes were found in the file: " << address_book << "."
                << "\n\tThere are no network nodes to monitor. Terminating program.\n\n";
        std::exit(1);
    }

    return n;
}

//-------------------------------------------------------------------------

void dtUtil::write_address_book(std::vector<Node>& nodes,std::string address_book)
{
    std::ofstream ofs{address_book};    
    if(!ofs){
        std::cerr << "\n\n\tERROR: Cannot open file: " << address_book << " for writing.";
        return;
    }

    for(Node n : nodes){
        ofs << n.name << " : " << n.address << '\n';
    }         
} 

//-------------------------------------------------------------------------

bool dtUtil::node_name_grtr(const Node& a,const Node& b)
{
    return a.name > b.name;
}

//-------------------------------------------------------------------------

bool dtUtil::node_name_lssr(const Node& a,const Node& b)
{
    return a.name < b.name;
}

//-------------------------------------------------------------------------

bool dtUtil::node_address_grtr(const Node& a,const Node& b)
{
    return a.address > b.address;
}

//-------------------------------------------------------------------------

bool dtUtil::node_address_lssr(const Node& a,const Node& b)
{
    return a.address < b.address;
}

//-------------------------------------------------------------------------

//Utilities for configuration
//-------------------------------------------------------------------------

//TODO: fix dtUtil::enter_log
void dtUtil::enter_log(std::string entry)
{
    const char sep = '/';
    entry = Poco::replace(entry,"\n"," ");
    std::string log_time_stamp = Poco::DateTimeFormatter::format(Poco::LocalDateTime(),"%Y%m%d");
    std::string entry_time_stamp = Poco::DateTimeFormatter::format(Poco::LocalDateTime(),"%Y%m%d-%H:%M:%s");

    std::ofstream ofs{
        read_config_file().get_log_loc()
        + sep 
        + log_time_stamp 
        + "-downtime.log",
        std::ios_base::app};

    ofs << entry_time_stamp << " " << entry << std::endl;
}

//-------------------------------------------------------------------------

Configuration dtUtil::read_config_file()
{
    //headers found in config file
    const std::string EMAIL_HOST = "email_host";
    const std::string SEND_AS = "sender_email";
    const std::string SEND_TO = "recipient_email";
    const std::string INTERVAL = "wait_for";
    const std::string MAX_FAIL = "max_fails";
    const std::string LOG_DIR = "log_dir";

    //the information assosiated with the above constants
    std::string email_host;
    std::string send_as;
    std::string send_to;

    //optional variables initialized with default values
    int interval = 60;
    int max_fail = 5;

    std::string log_dir = dtUtil::DEFAULT_LOG_PATH;

    std::ifstream ifs{dtUtil::CONFIG_FILE};
    if(!ifs){
        Configuration conf{"","","",interval,max_fail,log_dir};
        return conf;
    }

    const char sep = ':';
    const char comment = '#';
    std::string line;
    while(std::getline(ifs,line)){
        std::string header;
        std::string val;
        std::istringstream is{line};
        for(char ch;is.get(ch);){
            if(ch == comment) break;
            if(ch != sep){
                header += ch;
            }else{
                if(Poco::trim(header) == EMAIL_HOST) {is.get(ch); is >> email_host;}
                if(Poco::trim(header) == SEND_AS) {is.get(ch); is >> send_as;}
                if(Poco::trim(header) == SEND_TO) {is.get(ch); is >> send_to;}
                if(Poco::trim(header) == INTERVAL) {is.get(ch); is >> interval;}
                if(Poco::trim(header) == MAX_FAIL) {is.get(ch); is >> max_fail;}
                if(Poco::trim(header) == LOG_DIR) {is.get(ch); is >> log_dir;}
            }
        }
    }

    Configuration config{
        Poco::trim(email_host),
        Poco::trim(send_as),
        Poco::trim(send_to),
        interval,
        max_fail,
        log_dir};

    return config;
}

//-------------------------------------------------------------------------

Configuration dtUtil::load_config_file()
{
    //headers found in config file
    const std::string EMAIL_HOST = "email_host";
    const std::string SEND_AS = "sender_email";
    const std::string SEND_TO = "recipient_email";
    const std::string INTERVAL = "wait_for";
    const std::string MAX_FAIL = "max_fails";
    const std::string LOG_DIR = "log_dir";

    Configuration config{dtUtil::read_config_file()};

    //checks for empty required variables
    if(config.get_host().empty()){
        std::cerr << "\n\n\tERROR: Configuration file missing required variable: " << EMAIL_HOST
                    << " Cannot start program.\n\n";
        std::exit(1);
    }

    if(config.get_sender().empty()){
        std::cerr << "\n\n\tERROR: Configuration file missing required variable: " << SEND_AS
                    << " Cannot start program.\n\n";
        std::exit(1);
    }

    if(config.get_recipient().empty()){
        std::cerr << "\n\n\tERROR: Configuration file missing required variable: " << SEND_TO
                    << " Cannot start program.\n\n";
        std::exit(1);
    }

    return config;
}

//-------------------------------------------------------------------------

void dtUtil::write_config_file(Configuration& config)
{
    //headers found in config file
    const std::string EMAIL_HOST = "email_host";
    const std::string SEND_AS = "sender_email";
    const std::string SEND_TO = "recipient_email";
    const std::string INTERVAL = "wait_for";
    const std::string MAX_FAIL = "max_fails";
    const std::string LOG_DIR = "log_dir";

    const std::string sep = " : ";
    const std::string com = "# ";
    
    std::stringstream help_msg;
    help_msg << com << "Downtime network monitoring configuration file." << std::endl
        << com << std::endl
        << com << "The file is made up of variable names and their values seperated by a colon (':')." 
        << std::endl
        << com << "Variables do not have spaces in their names. Spaces are replaced with underscores ('_')."
        << std::endl
        << com << "Variables must be lower case." << std::endl
        << com << std::endl
        << com << "The Downtime config file has required and optional variables described below." << std::endl
        << com << "The required variables must be included in the config file and have a value set." 
        << std::endl
        << com << "The optional variables can be safely omited from the config file if the default values" 
        << std::endl
        << com << "are acceptable." << std::endl
        << com << "Comments can be added by prefixing a hash-tag ('#') to the beginning of the comment." 
        << std::endl
        << com << std::endl
        << com << "Required variables:" << std::endl
        << com << "\temail_host --> is the server alerts will be sent to." 
        << " The value can be in FQDN or IP address format." << std::endl
        << com << "\t\tExample: FQDN: mail.example.com | IP: 192.168.1.230" << std::endl
        << com << "\tsender_email --> is the email address you want displayed in the \"From\" field of an email."
        << std::endl
        << com << "\trecipient_email --> is the email address you want the alert delivered to." << std::endl
        << com << std::endl
        << com << "Optional variables:" << std::endl
        << com << "\twait_for --> is the period in seconds Downtime will wait between sending pings."
        << " DEFAULT: 60." << std::endl
        << com << "\tmax_failes --> is the number of consecutive failed responses counted against a node"
        << std::endl
        << com << "\t\tbefore an alert is generated. DEFAULT: 5." << std::endl
        << com << "\tlog_dir --> is the directory where Downtime log files will be kept."
        << " DEFAULT: /opt/downtime/logs/." << std::endl;

    std::ofstream ofs{dtUtil::CONFIG_FILE};

    ofs << help_msg.str() << std::endl
        << EMAIL_HOST << sep << config.get_host() << std::endl
        << SEND_AS << sep << config.get_sender() << std::endl
        << SEND_TO << sep << config.get_recipient() << std::endl
        << INTERVAL << sep << config.get_interval() << std::endl
        << MAX_FAIL << sep << config.get_max_fail() << std::endl
        << LOG_DIR << sep << config.get_log_loc() << std::endl;
}

//Utilities for logs
//-------------------------------------------------------------------------

std::vector<Log> dtUtil::read_log(std::string log) //returns emptry vector if no file found
{
    std::vector<Log> logs;
    
    std::ifstream ifs{log};

    if(!ifs){
        return logs;
    }else{
        std::string line;
        while(std::getline(ifs,line)){
            std::string t = line.substr(0,25);
            std::string e = line.substr(25);

            Log l{t,e};
            logs.push_back(l);
        }
    }
    
    return logs;
}

//-------------------------------------------------------------------------

bool dtUtil::log_time_grtr(const Log& a,const Log& b)
{
    return a.time > b.time;
}

bool dtUtil::log_time_lssr(const Log& a,const Log& b)
{
    return a.time < b.time;
}

bool dtUtil::log_entry_grtr(const Log& a,const Log& b)
{
    return a.entry > b.entry;
}

bool dtUtil::log_entry_lssr(const Log& a,const Log& b)
{
    return a.entry < b.entry;
}

//Misc Utilities
//-------------------------------------------------------------------------

void dtUtil::cstr_to_std(std::string& s)
{
    const char eol = '\0';

    std::string s2;
    for(char c : s){
        if(c == eol) break;
        s2 += c;
    }

    s = s2;
}

//-------------------------------------------------------------------------

bool dtUtil::is_running(std::string& pid)
{
    Poco::Path proc_dir = dtUtil::DEFAULT_PROC_PATH;
    Poco::DirectoryIterator di{proc_dir};
    Poco::DirectoryIterator end;

    for(di; di != end; ++di){
        if(di->isDirectory()){
            Poco::Path p = proc_dir;
            p.append(di.name());

            Poco::File f{p.append(dtUtil::DEFAULT_PROC_FILE)};
            if(f.exists()){
                std::ifstream ifs{f.path()};
                std::string line;
                std::getline(ifs,line);

                dtUtil::cstr_to_std(line);

                if(line == dtUtil::DEFAULT_EXE_PATH){
                    pid = di.name();
                    return true;
                }
            }
        }
    }

    pid = "0";
    return false;
}

//-------------------------------------------------------------------------

std::string dtUtil::to_upper(std::string ls)
{
    std::string us;
    for(char c : ls){
        us += std::toupper(c);
    }
    return us;
}

//-------------------------------------------------------------------------

void dtUtil::sleep(int t)
{
    std::this_thread::sleep_for(std::chrono::seconds(t));
}


