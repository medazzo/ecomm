#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>

#include "EasinCommBi.h"
#include "EasinCommand.h"

EasinCommBi * line;

int MAX_count=500000 ;

int play(EasinCommBi * line){
    int count = 1 ;
    int id=0;
    std::vector<std::string> vc;
    vc.push_back("---");
    vc.push_back("+++");
    usleep(2000000);
    while(count<MAX_count){
        id=count*555;
        if(line->IsInversed()) 
            std::cout<<"Write_on_IN :["<<count<<"/"<<MAX_count<<"] : Sending .. "<<id<<"." <<std::endl;
        else
            std::cout<<"Write_on_OUT:["<<count<<"/"<<MAX_count<<"] : Sending .. "<<id<<"." <<std::endl;
        line->SendCommand(EasinCommand(std::to_string(id),"cmd",vc));   
        count++;
        // minimum time to wait for; unless queue will miss somes messages (not a problem for Fifo) !
        usleep(500);
    }
    std::cout << "Send ALL !"<< std::endl;
    return 0;
}
int receiveFifo(EasinCommBi * line){
    int count = 1 ;
    while(count<MAX_count){
        EasinCommand * cmd = line->ReceiveCommand();
        if(cmd  != NULL){
            count ++;
            if(line->IsInversed()) 
                std::cout<<"Read_on_OUT :["<<count<<"/"<<MAX_count<<"] : Recevied  ID "<< cmd->m_id <<" >>."<<std::endl;
            else
                std::cout<<"Read_on_IN  :["<<count<<"/"<<MAX_count<<"] : Recevied  ID "<< cmd->m_id <<" >>."<<std::endl;
            delete cmd;
        }else {
            std::cout<<"["<<count<<"/"<<MAX_count<<"] : ERROR  NULL, END of Stream :: quit >>."<<std::endl;
            return -1;
        }
    }
    std::cout << "Received ALL !"<< std::endl;
    return 0;
}

//#define USE_MSG_QUE 1 // duplex is very bad with mQueue
#ifdef USE_MSG_QUE
#define pathi "/home/medazzo/test-tmp.sh"
#define patho "/home/medazzo/stale_outputs_checked"
#else
#define pathi "/tmp/Igifo"
#define patho "/tmp/Ogifo"
#endif
int main(int argc, char **argv) {    
     std::cout << "!   ! !! Welcome " <<argc<< std::endl;
    if(argc == 1 ){ // No prameter Call !
        line = new EasinCommBi(pathi, patho, false );      
    }
    else {
        line = new EasinCommBi(pathi, patho, true);
    }

    if(line->Initialize() < 0){
        std::cout << "Error to initiate CommBi !" << std::endl;
        return -1;
    }
    std::cout << "Initilized correctly CommBi ! " << std::endl;
    std::thread testR(receiveFifo,line);        
    std::thread testW(play, line);        
    testW.join();
    testR.join();
    if(line->Terminate() < 0){
        std::cout << "Error to initiate In CommBi !" << std::endl;
        return -1;
    }        

    std::cout << "Bye !"<< std::endl;
    usleep(100000);
    return 0;
}
