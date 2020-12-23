#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <condition_variable>

#ifndef EPOS_COMMAND_H
#define EPOS_COMMAND_H

#define END_STRING "END;"
#define END_STRING_LENGTH 4
#define SEP_STRING ";"


class EposCommand {
public:
    EposCommand(std::string id,std::string command,std::vector<std::string> & params );
    virtual ~EposCommand();
    std::string Serialize();
    static EposCommand * Deserialize(std::string command);
 
    std::string m_id;
    std::string m_command;
    std::vector<std::string> m_params;
};



#endif /* EPOS_COMMAND_H */

