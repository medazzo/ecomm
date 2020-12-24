#include <iostream>
#include <string>

#include "EposCommand.h"

#ifndef EPOS_COMM_H
#define EPOS_COMM_H

enum eposMode{
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


class EposComm {
 
public:   
    EposComm(std::string basePath, eposMode mode = eposMode::ECOMM_READ);    
    virtual ~EposComm();

    int Initialize();
    int Terminate();
    int SendCommand(EposCommand  command, long type = 1);
    EposCommand * ReceiveCommand(long type = 1);

private:    
    std::string m_path;
    std::string m_received;
    eposMode    m_mode;
#ifdef  USE_MSG_QUE
    int m_qid;
#else
    int m_fdWrite;
    int m_fdRead;
#endif

};


#endif /* EPOS_COMM_H */

