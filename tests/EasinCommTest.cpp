#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>

#include "EasinComm.h"
#include "EasinCommand.h"

int MAX_count=1000000 ;
int count = 1 ;
EasinComm<EasinCommand> * Fifo = NULL;

int play(){
    int id=0;
    std::vector<std::string> vc;
    vc.push_back("---");
    vc.push_back("+++");
    usleep(2000000);
    while(count<MAX_count){
        id=count*555;
        std::cout<<"["<<count<<"/"<<MAX_count<<"] : Sending .. "<<id<<"." <<std::endl;        
        EasinCommand * t = new EasinCommand(std::to_string(id),"cmd",vc);
        Fifo->SendCommand(t);   
        count++;
        // minimum time to wait for; unless queue will miss somes messages (not a problem for Fifo) !
        usleep(500);
        delete t;
    }
    std::cout << "Send ALL !"<< std::endl;
    return 0;
}
int receiveFifo(){
   while(count<MAX_count){
        EasinCommand * cmd = Fifo->ReceiveCommand();
        if(cmd  != NULL){
            count ++;
            std::cout<<"["<<count<<"/"<<MAX_count<<"] : Recevied  ID "<< cmd->getId() <<" >>."<<std::endl;
        }else {
            std::cout<<"["<<count<<"/"<<MAX_count<<"] : ERROR  NULL, END of Stream :: quit >>."<<std::endl;
            return -1;
        }
    }
    std::cout << "Received ALL !"<< std::endl;
    return 0;
}

#ifdef USE_MESSAGE_QUEUE
#define path "/etc"
#else
#define path "/tmp/fgifo"
#endif
int main(int argc, char **argv) {
    if(argc == 1 || argc > 2  ){
        std::cout << "Half duplex ! Please chose Mode : RECEIVER | SENDER !!"<<std::endl;
        return -1;
    }
    if(std::string(argv[1]) == "RECEIVER" ){
        Fifo = new EasinComm<EasinCommand>(path,EasinMode::ECOMM_READ);
        if(Fifo->Initialize() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
        std::cout << "Initilized correctly READ Mode ! " << std::endl;
        std::thread test(receiveFifo);
        test.join();
    }
    else if(std::string(argv[1]) == "SENDER" ){
        Fifo = new EasinComm<EasinCommand>(path ,EasinMode::ECOMM_WRITE);
        if(Fifo->Initialize() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
        std::cout << "Initilized correctly WRITE Mode ! " << std::endl;
        std::thread test(play);
        test.join();
    }else {     
        std::cout << " !! Pelase chose Mode : RECEIVER | SENDER !!"<<std::endl;
        return -1;    
    }
    
    std::cout << "Bye !"<< std::endl;
    usleep(100000);
    return 0;
}
