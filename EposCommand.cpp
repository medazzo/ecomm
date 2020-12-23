#include "EposCommand.h"


EposCommand::EposCommand(std::string id,
                                           std::string command,
                                           std::vector<std::string> & params )
{
    m_id= id ;
    m_command = command ;
    m_params = params ;
}

EposCommand::~EposCommand(){

}

std::string EposCommand::Serialize()
{
    std::string  params="";
    std::cout<< "------- Serializing  .. "<<m_params.size()<<std::endl;
    for (int i=0;i<m_params.size();i++)
        params+= m_params[i] + SEP_STRING;
    std::cout<< "------- Serializing  .. " <<std::endl;
    return std::string(m_id + SEP_STRING + m_command + SEP_STRING + params + END_STRING);
}
        
EposCommand * EposCommand::Deserialize(std::string command)
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
    return new EposCommand(id, cmd , strings);
}