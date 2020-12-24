#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>

#include "EposComm.h"
#include "EposCommand.h"

EposComm * lineIn;
EposComm * lineOut;

int MAX_count=5000 ;

int play(EposComm * line){
    int count = 1 ;
    int id=0;
    std::vector<std::string> vc;
    vc.push_back("---");
    vc.push_back("+++");
    usleep(2000000);
    while(count<MAX_count){
        id=count*555;
        if(line==lineIn) 
            std::cout<<"lineIn :["<<count<<"/"<<MAX_count<<"] : Sending .. "<<id<<"." <<std::endl;
        else
            std::cout<<"lineOut :["<<count<<"/"<<MAX_count<<"] : Sending .. "<<id<<"." <<std::endl;
        line->SendCommand(EposCommand(std::to_string(id),"cmd",vc));   
        count++;
        // minimum time to wait for; unless queue will miss somes messages (not a problem for Fifo) !
        usleep(500);
    }
    std::cout << "Send ALL !"<< std::endl;
    return 0;
}
int receiveFifo(EposComm * line){
    int count = 1 ;
    while(count<MAX_count){
        EposCommand * cmd = line->ReceiveCommand();
        if(cmd  != NULL){
            count ++;
            if(line==lineIn) 
                std::cout<<"lineIn :["<<count<<"/"<<MAX_count<<"] : Recevied  ID "<< cmd->m_id <<" >>."<<std::endl;
            else
                std::cout<<"lineOut :["<<count<<"/"<<MAX_count<<"] : Recevied  ID "<< cmd->m_id <<" >>."<<std::endl;
        }else {
            std::cout<<"["<<count<<"/"<<MAX_count<<"] : ERROR  NULL, END of Stream :: quit >>."<<std::endl;
            return -1;
        }
    }
    std::cout << "Received ALL !"<< std::endl;
    return 0;
}

// #define USE_MSG_QUE 1 // duplex is very bad with mQueue
#ifdef USE_MSG_QUE
#define pathIn "/etc"
#define pathOut "/tmp"
#else
#define pathIn "/tmp/Igifo"
#define pathOut "/tmp/Ogifo"
#endif
int main(int argc, char **argv) {    
    if(argc == 1 ){// This process then use Line In to Read and LineOut To Write 
        // lineIn is on Read  
        lineIn = new EposComm(pathIn,eposMode::ECOMM_READ);
        if(lineIn->Initialize() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
        std::cout << "Initilized correctly READ Mode ! " << std::endl;
        std::thread testR(receiveFifo,lineIn);        
        // lineOut is on Write
        lineOut = new EposComm(pathOut ,eposMode::ECOMM_WRITE);
        if(lineOut->Initialize() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
        std::cout << "Initilized correctly WRITE Mode ! " << std::endl;
        std::thread testW(play, lineOut);        
        testW.join();
        testR.join();
        if(lineIn->Terminate() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
         if(lineOut->Terminate() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
    }
    else {// This process then use Line Out to Write and LineIn To Read
        // lineIn is on Write  
        lineIn = new EposComm(pathIn ,eposMode::ECOMM_WRITE);
        if(lineIn->Initialize() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
        std::cout << "Initilized correctly WRITE Mode ! " << std::endl;
        std::thread testW(play, lineIn);        
        // lineOut is on Read  
        lineOut = new EposComm(pathOut,eposMode::ECOMM_READ);
        if(lineOut->Initialize() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
        std::cout << "Initilized correctly READ Mode ! " << std::endl;
        std::thread testR(receiveFifo,lineOut);
        testW.join();
        testR.join();
        if(lineIn->Terminate() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
        if(lineOut->Terminate() < 0){
            std::cout << "Error to initiate In Fifo !" << std::endl;
            return -1;
        }
    }
    
    std::cout << "Bye !"<< std::endl;
    usleep(100000);
    return 0;
}
