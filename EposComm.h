#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <condition_variable>

#include "EposCommand.h"

#ifndef EPOS_COMM_H
#define EPOS_COMM_H



class EposComm {
 
public:   
    EposComm(std::string path);    
    virtual ~EposComm();

    int Initialize();
    int Terminate();
    int SendCommand(EposCommand  command);
    EposCommand * ReceiveCommand();

private:    
    std::string m_path;
    int m_fdWrite;
    int m_fdRead;
    bool m_stop;
    std::mutex                   m_mutex;
    std::deque<EposCommand*> m_queue;
    std::condition_variable      m_cond;
    int  ListenCommands();
    void StartListener();    
};


#endif /* EPOS_COMM_H */

