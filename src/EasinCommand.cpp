#include "EasinCommand.h"


EasinCommand::EasinCommand(std::string id,
                                           std::string command,
                                           std::vector<std::string> & params )
{
    m_id= id ;
    m_command = command ;
    m_params = params ;
}

EasinCommand::~EasinCommand(){

}

std::string EasinCommand::Serialize()
{
    std::string  params="";
    for (int i=0;i<m_params.size();i++)
        params+= m_params[i] + SEP_STRING;
    return std::string(m_id + SEP_STRING + m_command + SEP_STRING + params + END_STRING);
}
        
EasinCommand * EasinCommand::Deserialize(std::string command)
{
    std::vector<std::string> strings;
    std::istringstream f(command);
    std::string s,id,cmd;  
    int i=0;
    while (getline(f, s, ';')) {
        if(i == 0)
            id = s;
        else if(i==1)
            cmd = s ;
        else
            strings.push_back(s);
        i++;
    }
    return new EasinCommand(id, cmd , strings);
}