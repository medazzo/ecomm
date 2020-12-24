#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstring>

key_t my_key;
int msg_id;
bool stop = false ;

// structure for message queue
class msg_buffer {
public:
   long msg_type;
   char msg[100];
} message;


int Send(msg_buffer message) {
   msg_id = msgget(my_key, 0666 | IPC_CREAT); //create message queue and return id      
   msgsnd(msg_id, &message, sizeof(message), 0); //send message
   printf("Sent message is : %s \n", message.msg);
   return 0;
}

int Receive() {   
    while(!stop){
        msg_id = msgget(my_key, 0666 | IPC_CREAT); //create message queue and return id
        msgrcv(msg_id, &message, sizeof(message), 1, 0); //used to receive message
        printf("Received Message is : %s \n", message.msg);        
    }
    std::cout << "Done Receiving !" << std::endl;
   return 0;
}


int play(){
    msg_buffer msg;
    msg.msg_type = 1 ;
    memcpy(msg.msg,"fklmj,oil;fghj;fgh;fgh;jsdfdsflknljsdf",99);
    for(int i=0;i<100;i++){
        Send(msg);                
    }
    std::cout << "Done Sending !" << std::endl;
    return 0;
}


int main(int argc, char **argv) {

    my_key = ftok("progfile", 65); //create unique key
    std::cout << "Initilized correctly !" << std::endl;

    std::thread test1(Receive);    
    std::thread test(play);
    test.join();
    stop = true ;
   /* msg_buffer msg;
    msg.msg_type = 1 ;
    memcpy(msg.msg,"LAst !!f",99);
    Send(msg);  */
    //usleep(1000000);
    msgctl(msg_id, IPC_RMID, NULL); //destroy the message queue
    return 0;
}
