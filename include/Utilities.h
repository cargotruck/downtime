//
// Utilities.h
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Path.h"
#include "Poco/Process.h"

#include <wx/wx.h>

#include "./Configuration.h"
#include "./Log.h"
#include "./Node.h"

namespace dtUtil
{
//START GLOBAL CONSTANTS -- pulled from main.cpp

const std::string CONFIG_FILE = "/opt/downtime/downtime.config";
const std::string DEFAULT_ADDRESS_BOOK_PATH = "/opt/downtime/address_book";
const std::string DEFAULT_LOG_PATH = "/opt/downtime/logs/";
const std::string DEFAULT_EXE_PATH = "/opt/downtime/downtime";
const std::string DEFAULT_PROC_PATH = "/proc";
const std::string DEFAULT_PROC_FILE = "cmdline";
 
const std::string BAD_ADDRESS_FLAG = "-1";

//END GLOBAL CONSTANTS

//Utilities for addressbook
std::vector<Node> read_address_book(std::string address_book = DEFAULT_ADDRESS_BOOK_PATH);
void write_address_book(std::vector<Node>& nodes,std::string address_book = DEFAULT_ADDRESS_BOOK_PATH);

bool node_name_grtr(const Node& a,const Node& b);
bool node_name_lssr(const Node& a,const Node& b);
bool node_address_grtr(const Node& a,const Node& b);
bool node_address_lssr(const Node& a,const Node& b);


//Utilities for configuration
Configuration read_config_file(); //for program configuration, can return empty Configuration
Configuration load_config_file(); //for program execution, cannot have empty reqs
void write_config_file(Configuration& config);

//Utilities for Logs
void enter_log(std::string entry);
std::vector<Log> read_log(std::string log = DEFAULT_LOG_PATH); //TODO: the default is misleading, is dir, not file

bool log_time_grtr(const Log& a,const Log& b);
bool log_time_lssr(const Log& a,const Log& b);
bool log_entry_grtr(const Log& a,const Log& b);
bool log_entry_lssr(const Log& a,const Log& b);

//Misc utilities
void cstr_to_std(std::string& s);
bool is_running(std::string& pid);
std::string to_upper(std::string ls);

void sleep(int t);

} //end namespace dtUtil
#endif //UTILITIES_H
