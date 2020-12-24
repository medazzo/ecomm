#include <iostream>
#include <string>

#include "EasinCommand.h"

#ifndef Easin_COMM_H
#define Easin_COMM_H

enum EasinMode{
    ECOMM_READ = 0 ,
    ECOMM_WRITE
} ;


#ifdef  USE_MSG_QUE

#define ProjectId 123
#define MsgLen    1024
#define MsgCount  6

typedef struct {
  long type;                 /* must be of type long */
  char payload[MsgLen + 1];  /* bytes in the message */
} queuedMessage;

#endif


class EasinComm {
 
public:   
    EasinComm(std::string basePath, EasinMode mode = EasinMode::ECOMM_READ);    
    virtual ~EasinComm();

    int Initialize();
    int Terminate();
    int SendCommand(EasinCommand  command, long type = 1);
    EasinCommand * ReceiveCommand(long type = 1);

private:    
    std::string m_path;
    std::string m_received;
    EasinMode    m_mode;
#ifdef  USE_MSG_QUE
    int m_qid;
#else
    int m_fdWrite;
    int m_fdRead;
#endif

};


#endif /* Easin_COMM_H */

