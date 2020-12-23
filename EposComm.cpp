#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include <thread>
#include "EposComm.h"


#define BUFFSIZE 512




EposComm::EposComm(std::string path):
m_path(path),
m_stop(false)
{
    mkfifo(m_path.c_str(),0600);    
}
EposComm::~EposComm()
{
    unlink(m_path.c_str());
}
int EposComm::Initialize()
{
    if ( (m_fdWrite=open(m_path.c_str(), O_WRONLY)) < 0){
        std::cout << " Fail to open Write fd on "<<m_path << " !";
        return -1;
    }
    if ( (m_fdRead=open(m_path.c_str(), O_RDONLY)) < 0){
        std::cout << " Fail to open Read fd on "<<m_path << " !";
        return -2;
    }
    return 0;
}
int EposComm::Terminate()
{
    close(m_fdWrite);
    close(m_fdRead);
    return 0;
}
int EposComm::SendCommand(EposCommand  command)
{
    std::string cmd = command.Serialize();
     if( write(m_fdWrite, cmd.c_str(), cmd.size() ) != cmd.size() ) {
        std::cout << " Fail Write "<<cmd.size()<<" octect  on "<<m_path << " !";
        return -1;
    }
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

    while(!m_stop || (n=read(m_fdRead, buff , BUFFSIZE) ) > 0){
        std::cout<<" <-- Received  ... "<< n;        
        received.append(buff,n);
        std::cout<<" <-- ALL Received  ... "<< received;
        //Find END; ?
        while ((found = received.find(END_STRING)) != std::string::npos) {
            std::cout << "first 'END_STRING' found at: " << found ;
            token = received.substr(0, found);            
            std::cout << " COMMAND :"<< token << std::endl;
            received.erase(0, found + END_STRING_LENGTH);
            //push to queue !
            {            
                std::unique_lock<std::mutex> lock(this->m_mutex);
                m_queue.push_front(EposCommand::Deserialize(token));
            }
            this->m_cond.notify_one();
        }
    }
    return 0;
}
void EposComm::StartListener()
{
    // Start thread
	std::thread read(&EposComm::ListenCommands, this);	
}