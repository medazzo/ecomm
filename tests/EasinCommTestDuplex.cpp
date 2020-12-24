#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>

#include "EasinCommBi.h"
#include "EasinCommand.h"

EasinCommBi<EasinCommand> * line;

int MAX_count=10 ;

int play(EasinCommBi<EasinCommand> * line){
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
        EasinCommand* t = new EasinCommand(std::to_string(id),"cmd",vc);
        line->SendCommand(t);   
        count++;
        // minimum time to wait for; unless queue will miss somes messages (not a problem for Fifo) !
        usleep(500);
        delete t ;
    }
    std::cout << "Send ALL !"<< std::endl;
    return 0;
}
int receiveFifo(EasinCommBi<EasinCommand> * line){
    int count = 1 ;
    while(count<MAX_count){
        EasinCommand * cmd = line->ReceiveCommand();
        if(cmd  != NULL){
            count ++;
            if(line->IsInversed()) 
                std::cout<<"Read_on_OUT :["<<count<<"/"<<MAX_count<<"] : Recevied  ID "<< cmd->getId() <<" >>."<<std::endl;
            else
                std::cout<<"Read_on_IN  :["<<count<<"/"<<MAX_count<<"] : Recevied  ID "<< cmd->getId() <<" >>."<<std::endl;
            delete cmd;
        }else {
            std::cout<<"["<<count<<"/"<<MAX_count<<"] : ERROR  NULL, END of Stream :: quit >>."<<std::endl;
            return -1;
        }
    }
    std::cout << "Received ALL !"<< std::endl;
    return 0;
}

#ifdef USE_MESSAGE_QUEUE
#define pathi "~/.profile"
#define patho "~/.bashrc"
#else
#define pathi "/tmp/Igifo"
#define patho "/tmp/Ogifo"
#endif
int main(int argc, char **argv) {        
    if(argc == 1 || argc > 2  ){
        std::cout << "Full duplex ! Please pick a Side  : SIDE1 | SIDE2 !!"<<std::endl;
        return -1;
    }
    if(std::string(argv[1]) == "SIDE1" ){
        line = new EasinCommBi<EasinCommand>(pathi, patho, false );      
    }
    else if(std::string(argv[1]) == "SIDE2" ){
        line = new EasinCommBi<EasinCommand>(pathi, patho, true);
    }else {
        std::cout << "Full duplex ! Please pick a Side  : SIDE1 | SIDE2 !!"<<std::endl;
        return -1;
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
