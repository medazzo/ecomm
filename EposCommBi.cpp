#include "EposCommBi.h"
#include <unistd.h>


#define IN_PREFIX "-IN"
#define OUT_PREFIX "-OUT"

EposCommBi::EposCommBi(std::string basePathIn, std::string basePathOut, bool reverseDirection):
m_reverseDirection(reverseDirection),
m_basePathIn(basePathIn),
m_basePathOut(basePathOut)
{
    if(m_reverseDirection) {
        // OUT is on Read  
        m_lineRead = new EposComm(m_basePathOut, eposMode::ECOMM_READ);
        // IN is on Write
        m_lineWrite = new EposComm(m_basePathIn, eposMode::ECOMM_WRITE);
    }else {
        // IN is on Read  
        m_lineRead = new EposComm(m_basePathIn, eposMode::ECOMM_READ);
        // OUTis on Write
        m_lineWrite = new EposComm(m_basePathOut, eposMode::ECOMM_WRITE);
    }
}

EposCommBi::EposCommBi(std::string basePath,  bool reverseDirection):
m_reverseDirection(reverseDirection),
m_basePathIn(basePath+IN_PREFIX),
m_basePathOut(basePath+OUT_PREFIX)
{
    if(m_reverseDirection) {
        // OUT_PREFIX is on Read  
        m_lineRead = new EposComm(m_basePathOut, eposMode::ECOMM_READ);
        // IN_PREFIX is on Write
        m_lineWrite = new EposComm(m_basePathIn, eposMode::ECOMM_WRITE);
    }else {
        // IN_PREFIX is on Read  
        m_lineRead = new EposComm(m_basePathIn, eposMode::ECOMM_READ);
        // OUT_PREFIX is on Write
        m_lineWrite = new EposComm(m_basePathOut, eposMode::ECOMM_WRITE);
    }
}
EposCommBi::~EposCommBi()
{    
    delete m_lineRead;
    delete m_lineWrite;
}
int EposCommBi::Initialize()
{   
    int ntry = 0 , loop=50;
    bool fail = false ;
    long slept = 500000;
    // Not inversing order will result in mutuel exclusion,
    if(!m_reverseDirection) {
        std::cout << "Waiting to Init WRITE comm ! " << std::endl; 
        if(m_lineWrite->Initialize() < 0){
            std::cout << "1: Error to initiate Write comm !" << std::endl;
            return -1;
        }        
        std::cout << "Initilized correctly WRITE comm ! " << std::endl; 
        // we could exit here because , the mkdid is not called when we do read , le's loop a little        
        while(ntry<loop){
            if(m_lineRead->Initialize() < 0){
                std::cout << "2: Error to initiate Read comm !, let's try again !" << std::endl;
                usleep(slept);
                fail = true ;
            }else {
                fail = false ;
                break;
            }
        }
        if(fail)
            return -1;
        std::cout << "Initilized correctly READ comm ! " << std::endl;  
    }else{
        std::cout << "Waiting to Init READ comm ! " << std::endl; 
        // we could exit here because , the mkdid is not called when we do read , le's loop a little        
        while(ntry<loop){
            if(m_lineRead->Initialize() < 0){
                std::cout << "1: Error to initiate Read comm !, let's try again !" << std::endl;
                usleep(slept);
                fail = true ;
            }else {
                fail = false ;
                break;
            }
        }
        if(fail)
            return -1;
        std::cout << "Initilized correctly READ comm ! " << std::endl;  

        std::cout << "Waiting to Init WRITE comm ! " << std::endl; 
        if(m_lineWrite->Initialize() < 0){
            std::cout << "2: Error to initiate Write comm !" << std::endl;
            return -1;
        }
        std::cout << "Initilized correctly WRITE comm ! " << std::endl; 
    }
    return 0;    
}
int EposCommBi::Terminate()
{
    if(m_lineRead->Terminate() < 0){
        std::cout << "Error to initiate Read comm !" << std::endl;
        return -1;
    }
    if(m_lineWrite->Terminate() < 0){
        std::cout << "Error to initiate Write comm !" << std::endl;
        return -1;
    }
    return 0;
}
int EposCommBi::SendCommand(EposCommand  command, long type)
{
    return  m_lineWrite->SendCommand(command ,type);
}
EposCommand * EposCommBi::ReceiveCommand(long type)
{  
    return  m_lineRead->ReceiveCommand(type);
}