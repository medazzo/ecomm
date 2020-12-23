#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>

#include "EposComm.h"
#include "EposCommand.h"

EposComm * In = NULL;
//EposComm * Out = NULL;

int play(){
    std::vector<std::string> vc;
    vc.push_back("ADSD");
    vc.push_back("BDXCVBDFV");

    for(int i=0;i<1000;i++){
        std::cout<< "------- Sending .. "<<i<<"." <<std::endl;
        In->SendCommand(
                EposCommand(
                    std::to_string(i+21542),
                    ">>>>->->->",
                    vc));
  /*      Out->SendCommand(
                EposCommand(
                    std::to_string(i+564872),
                    "<----<-<-<-",
                    vc));    
    */}
    return 0;
}
int receiveIn(){
   while(1){
       std::cout<< "------- [***IN***] Waiting .. "<<std::endl;
        EposCommand * cmd = In->ReceiveCommand();
        std::cout<< "------- [***IN***] Recevied Comand ID "<< cmd->m_id <<"."<<std::endl;
    }
    return 0;
}/*
int receiveOut(){
   while(1){
       std::cout<< "------- [***OUT***] Waiting .. "<<std::endl;
        EposCommand * cmd = Out->ReceiveCommand();
        std::cout<< "------ [***OUT***] Recevied Comand ID "<< cmd->m_id <<"."<<std::endl;
    }
    return 0;
}*/
int main(int argc, char **argv) {

    // (1)-->(2) Direction Fifo /tmp/fifo-IN
    In = new EposComm("/tmp/fifo-IN");
    if(In->Initialize() < 0){
        std::cout << "Error to initiate In Fifo !" << std::endl;
        return -1;
    }
    // (2)-->(1) Direction Fifo /tmp/fifo-OUT
   /* Out = new EposComm("/tmp/fifo-OUT");
    if(Out->Initialize() < 0){
        std::cout << "Error to initiate Out Fifo !" << std::endl;
        return -2;
    }
    */std::cout << "Initilized correctly !" << std::endl;

    std::thread test1(receiveIn);
    //std::thread test2(receiveOut);    
    std::thread test(play);
    test.join();
    usleep(1000000);
    return 0;
}
