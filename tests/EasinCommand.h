#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <condition_variable>

#ifndef Easin_COMMAND_H
#define Easin_COMMAND_H


class EasinCommand: public EasinCommObj {
public:
    EasinCommand(std::string id,std::string command,std::vector<std::string> & params ){
        m_id= id ;
        m_command = command ;
        m_params = params ;
    };

    EasinCommand(std::string command)
    {
        std::istringstream f(command);
        std::string s;  
        int i=0;
        while (getline(f, s, ';')) {
            if(i == 0)
                m_id = s;
            else if(i==1)
                m_command = s ;
            else
                m_params.push_back(s);
            i++;
        }
    };

    virtual ~EasinCommand(){

    };

    std::string Serialize(){
        std::string  params="";
        for (int i=0;i<m_params.size();i++)
            params+= m_params[i] + SEP_STRING;
        return std::string(m_id + SEP_STRING + m_command + SEP_STRING + params + END_STRING);
    };

    std::string getId(){ return m_id;};
private:
    std::string m_id;
    std::string m_command;
    std::vector<std::string> m_params;
};



#endif /* Easin_COMMAND_H */