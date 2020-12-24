#include <string>

#include "EposCommand.h"
#include "EposComm.h"

#ifndef EPOS_COMM_BI_H
#define EPOS_COMM_BI_H

class EposCommBi {
 
public:   
    EposCommBi(std::string basePath, bool reverseDirection);    
    EposCommBi(std::string basePathIn, std::string basePathOut, bool reverseDirection );    
    virtual ~EposCommBi();

    int Initialize();
    int Terminate();
    bool IsInversed(){return m_reverseDirection;}
    int SendCommand(EposCommand  command, long type = 1);
    EposCommand * ReceiveCommand(long type = 1);

private:    
    EposComm *  m_lineRead;   // Used for Read
    EposComm *  m_lineWrite;  // Used for Write
    // if true then inverse > m_lineRead for OUT_PREFIX and  m_lineWrite for IN_PREFIX
    bool m_reverseDirection; 
    std::string m_basePathIn;
    std::string m_basePathOut;
};


#endif /* EPOS_COMM_H */

