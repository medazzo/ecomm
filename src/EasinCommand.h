#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <condition_variable>

#ifndef Easin_COMMAND_H
#define Easin_COMMAND_H

#define END_STRING "END;"
#define END_STRING_LENGTH 4
#define SEP_STRING ";"


class EasinCommand {
public:
    EasinCommand(std::string id,std::string command,std::vector<std::string> & params );
    virtual ~EasinCommand();
    std::string Serialize();
    static EasinCommand * Deserialize(std::string command);
 
    std::string m_id;
    std::string m_command;
    std::vector<std::string> m_params;
};



#endif /* Easin_COMMAND_H */

