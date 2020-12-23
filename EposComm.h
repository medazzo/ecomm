#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "EposCommand.h"

#ifndef EPOS_COMM_H
#define EPOS_COMM_H

enum eposMode{
    ECOMM_SERVER = 0 ,
    ECOMM_CLIENT
} ;

class EposComm {
 
public:   
    EposComm(std::string basePath, eposMode mode = eposMode::ECOMM_SERVER);    
    virtual ~EposComm();

    int Initialize();
    int Terminate();
    int SendCommand(EposCommand  command);
    EposCommand * ReceiveCommand();

private:    
    std::string m_path;
    eposMode    m_mode;
    int m_fdWrite;
    int m_fdRead;
    bool m_stop;
    std::thread                 m_read;
    std::mutex                   m_mutex;
    std::deque<EposCommand*> m_queue;
    std::condition_variable      m_cond;
    int  ListenCommands();
};


#endif /* EPOS_COMM_H */

