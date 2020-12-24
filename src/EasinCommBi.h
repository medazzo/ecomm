#include <string>

#include "EasinCommand.h"
#include "EasinComm.h"

#ifndef Easin_COMM_BI_H
#define Easin_COMM_BI_H

class EasinCommBi {
 
public:   
    EasinCommBi(std::string basePath, bool reverseDirection);    
    EasinCommBi(std::string basePathIn, std::string basePathOut, bool reverseDirection );    
    virtual ~EasinCommBi();

    int Initialize();
    int Terminate();
    bool IsInversed(){return m_reverseDirection;}
    int SendCommand(EasinCommand  command, long type = 1);
    EasinCommand * ReceiveCommand(long type = 1);

private:    
    EasinComm *  m_lineRead;   // Used for Read
    EasinComm *  m_lineWrite;  // Used for Write
    // if true then inverse > m_lineRead for OUT_PREFIX and  m_lineWrite for IN_PREFIX
    bool m_reverseDirection; 
    std::string m_basePathIn;
    std::string m_basePathOut;
};


#endif /* Easin_COMM_H */

