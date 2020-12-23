#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "EposComm.h"

#define report_and_exit(msg) {perror(msg);exit(-1); /* EXIT_FAILURE */}


#define BUFFSIZE 5
#define IN_PREFIX "-IN"
#define OUT_PREFIX "-OUT"


EposComm::EposComm(std::string basePath, eposMode mode):
m_path(basePath),
m_mode(mode),
m_received("")
{
#ifdef  USE_MSG_QUE
    m_qid= 0 ;    
#else 
    m_fdWrite = 0;
    m_fdRead = 0;
#endif
}
EposComm::~EposComm()
{    
}
int EposComm::Initialize()
{
//   unlink(m_path.c_str());
#ifdef  USE_MSG_QUE
    key_t key = ftok(m_path.c_str(), ProjectId);
    if (key < 0) report_and_exit("couldn't get key...");

    m_qid = msgget(key, 0666 | IPC_CREAT);
    if (m_qid < 0) report_and_exit("couldn't get queue id...");
#else    
    if(m_mode == eposMode::ECOMM_WRITE) {
        /* create FIFO */
        if (mkfifo(m_path.c_str(), 0666) < 0)                 /* read/write for user/group/others */
        {
            if (errno == EEXIST) { // already exists, ok 
            }

            /* error */
            else {
                std::cout << "Epos comm mkfifo Fail on "<<m_path << " ."<< std::endl;
                return -1;
            }
        }  
    }
    if(m_mode == eposMode::ECOMM_READ) {
        // open fd read
        if ( (m_fdRead=open(m_path.c_str(), O_RDONLY)) < 0){
            std::cout << " Fail to open Read fd on "<<m_path << " !"<< std::endl;
            return -1;
        }
                
    }else {
        if ( (m_fdWrite=open(m_path.c_str(),O_CREAT | O_WRONLY)) < 0){
            std::cout << " Fail to open Write fd on "<<m_path << " !"<< std::endl;
            return -1;
        }
        std::cout << "WRITER Epos comm  Listener Correctly Launched on "<<m_path << " ."<< std::endl;
    }
#endif

    return 0;
}
int EposComm::Terminate()
{
#ifdef  USE_MSG_QUE       
    /** remove the queue **/
    if (msgctl(m_qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
        report_and_exit("trouble removing queue...");
#endif  
    if(m_mode == eposMode::ECOMM_READ) {        
        close(m_fdRead);
    }else {        
        close(m_fdWrite);         
    }      
    unlink(m_path.c_str());
    return 0;
}
int EposComm::SendCommand(EposCommand  command, long type)
{
    if(m_mode == eposMode::ECOMM_READ){
        std::cout << "Epos comm Fail to Send command in READ MODE !."<< std::endl;
        return -1;
    }
    std::string cmd = command.Serialize();    
#ifdef  USE_MSG_QUE    
    /* build the message */
    queuedMessage msg;
    msg.type = type;
    memset(msg.payload, 0 , MsgLen);
    strncpy(msg.payload, cmd.c_str(), cmd.size());
    /* send the message */
    msgsnd(m_qid, &msg, sizeof(msg), IPC_NOWAIT); /* don't block */
#else
   
   if( write(m_fdWrite, cmd.c_str(), cmd.size() ) != cmd.size() ) {
        std::cout << " Fail Write "<<cmd.size()<<" octect  on "<<m_path << " !"<< std::endl;
        return -1;
    }       
#endif    
    return 0;
}
EposCommand * EposComm::ReceiveCommand(long type){  
    if(m_mode == eposMode::ECOMM_WRITE){
        std::cout << "Epos comm Fail to Receive command in WRITE MODE !."<< std::endl;
        return NULL;
    }  
#ifdef  USE_MSG_QUE 
    queuedMessage msg; /* defined in queue.h */
    if (msgrcv(m_qid, &msg, sizeof(msg), type, MSG_NOERROR |IPC_NOWAIT ) < 0)
      puts("msgrcv trouble...\n");
    printf("%s received as type %i\n", msg.payload, (int) msg.type);
    return EposCommand::Deserialize(msg.payload);
#else
    ssize_t n=0;
    char buff[BUFFSIZE];
    std::size_t found = 0;
    std::string token ;

    //Find END; ?
    while ((found = m_received.find(END_STRING)) == std::string::npos) {
        n = read(m_fdRead, buff , BUFFSIZE) ;
        if (0 == n) {                  /* end of stream */
            std::cout<<"["<<m_path <<"] <-- END of Stream !  "<< std::endl;
            return NULL; 
        }        
        m_received.append(buff,n);        
        if((found = m_received.find(END_STRING)) != std::string::npos){
            token = m_received.substr(0, found);            
            m_received.erase(0, found + END_STRING_LENGTH);
            return EposCommand::Deserialize(token);
        }
    }  
    std::cout<<"["<<m_path <<"] <-- unknown error ! "<<m_received<<"."<< std::endl;
    return NULL;     
#endif
}