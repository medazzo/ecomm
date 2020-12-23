#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>

#include "EposComm.h"
#include "EposCommand.h"

int MAX_count=1001 ;
int count = 1 ;
EposComm * Fifo = NULL;

int play(){
    std::vector<std::string> vc;
    vc.push_back("---");
    vc.push_back("+++");
    usleep(2000000);
    while(count<MAX_count){
        std::cout<<"["<<count<<"/"<<MAX_count<<"] : Sending .. "<<count<<"." <<std::endl;        
        Fifo->SendCommand(
                EposCommand(
                    std::to_string(count),
                    ">>>>->->->",
                    vc));   
        count++;
    }
    std::cout << "Send ALL !"<< std::endl;
    return 0;
}
int receiveFifo(){
   while(count<MAX_count){
        EposCommand * cmd = Fifo->ReceiveCommand();
        count ++;
        std::cout<<"["<<count<<"/"<<MAX_count<<"] : Recevied  ID "<< cmd->m_id <<" ::>>."<<std::endl;
    }
    std::cout << "Received ALL !"<< std::endl;
    return 0;
}
#define path "/tmp"
int main(int argc, char **argv) {
    if(argc == 1 ){
        Fifo = new EposComm(path,eposMode::ECOMM_READ);
        if(Fifo->Initialize() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
        std::cout << "Initilized correctly READ Mode ! " << std::endl;
        std::thread test(receiveFifo);
        test.join();
    }
    else {
        Fifo = new EposComm(path ,eposMode::ECOMM_WRITE);
        if(Fifo->Initialize() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
        std::cout << "Initilized correctly WRITE Mode ! " << std::endl;
        std::thread test(play);
        test.join();
    }
    
    std::cout << "Bye !"<< std::endl;
    usleep(100000);
    return 0;
}
