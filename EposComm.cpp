#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include <thread>
#include "EposComm.h"


#define BUFFSIZE 512
#define IN_PREFIX "-IN"
#define OUT_PREFIX "-OUT"


EposComm::EposComm(std::string basePath, eposMode mode):
m_path(basePath),
m_mode(mode),
m_stop(false)
{
}
EposComm::~EposComm()
{    
}
int EposComm::Initialize()
{
    unlink(m_path.c_str());
    std::cout << "Epos comm unlinking old "<<m_path << " ."<< std::endl;
   
    /* create FIFO */
    if (mkfifo(m_path.c_str(), S_IRUSR | S_IWUSR) < 0) {
        if (errno == EEXIST) { // already exists, ok 
        }

        /* error */
        else {
            std::cout << "Epos comm mkfifo Fail on "<<m_path << " ."<< std::endl;
            return -1;
        }
    }
   
    // Start thread
	this->m_read = std::thread(&EposComm::ListenCommands, this);
    std::cout << "Epos comm  Listener Correctly Launched on "<<m_path << " ."<< std::endl;
    return 0;
}
int EposComm::Terminate()
{
    m_stop = true ;    
    unlink(m_path.c_str());
    return 0;
}
int EposComm::SendCommand(EposCommand  command)
{
    std::cout<< "------- SendCommand .. ." <<std::endl;
    // open write first !
    if ( (m_fdWrite=open(m_path.c_str(), O_WRONLY)) < 0){
        std::cout << " Fail to open Write fd on "<<m_path << " !"<< std::endl;
        return -1;
    }
    std::string cmd = command.Serialize();
    std::cout<< "------- SendCommand .."<<cmd<<" ." <<std::endl;
    std::cout <<"["<<m_path <<"] Will Send  "<<cmd.size()<<"   :\""<<cmd <<"\" !"<< std::endl;
     if( write(m_fdWrite, cmd.c_str(), cmd.size() ) != cmd.size() ) {
        std::cout << " Fail Write "<<cmd.size()<<" octect  on "<<m_path << " !"<< std::endl;
        return -1;
    }
    
    close(m_fdWrite);
    return 0;
}
EposCommand * EposComm::ReceiveCommand(){    
    std::unique_lock<std::mutex> lock(this->m_mutex);
    this->m_cond.wait(lock,[&]{return m_stop || !m_queue.empty();});
    if ( !this->m_queue.empty() ){        
        EposCommand * rc(std::move(this->m_queue.back()));
        this->m_queue.pop_back();
        return rc;
    }else{
        EposCommand * t;
        return t;       
    }        

}
int EposComm::ListenCommands()
{
    int n=0;
    char buff[BUFFSIZE];
    std::size_t found = 0;
    std::string token ;
    std::string received="";

      // open fd read
    if ( (m_fdRead=open(m_path.c_str(), O_RDONLY)) < 0){
        std::cout << " Fail to open Read fd on "<<m_path << " !"<< std::endl;
        return -2;
    }

    while(!m_stop && (n=read(m_fdRead, buff , BUFFSIZE) ) > 0){
        std::cout<<"["<<m_path <<"] <-- Received  ... "<< n<< std::endl;
        received.append(buff,n);
        //Find END; ?
        while ((found = received.find(END_STRING)) != std::string::npos) {
            token = received.substr(0, found);            
            received.erase(0, found + END_STRING_LENGTH);
            //push to queue !
            {            
                std::unique_lock<std::mutex> lock(this->m_mutex);
                m_queue.push_front(EposCommand::Deserialize(token));
            }
            this->m_cond.notify_one();
        }
    }

    close(m_fdRead);
    return 0;
}