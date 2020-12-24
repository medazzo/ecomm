#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstring>

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    100
bool stop = false ;



 
int Receive() {   
    char msg[MAX_SIZE];
    mqd_t mq;
    struct mq_attr attr;
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, &attr);
    if (mq == (mqd_t) -1) {
        perror(QUEUE_NAME);
        exit(EXIT_FAILURE);
    }

    while(!stop){
        mq_receive(mq, (char *)&msg, MAX_SIZE, NULL); //used to receive message
        printf("Received Message is : %s \n", msg);        
    }
    std::cout << "Done Receiving !" << std::endl;

    if ((mqd_t) -1 == mq_close(mq)) {
        perror(QUEUE_NAME);
        exit(EXIT_FAILURE);
    }
    if ((mqd_t) -1 == mq_unlink(QUEUE_NAME)){
        perror(QUEUE_NAME);
        exit(EXIT_FAILURE);
    }


   return 0;
}


int play(){
    char msg[MAX_SIZE];
    mqd_t mq;
    mq = mq_open(QUEUE_NAME, O_WRONLY );
    if (mq == (mqd_t) -1) {
        perror(QUEUE_NAME);
        exit(EXIT_FAILURE);
    }

    memcpy(msg,"fklmj,oil;fghj;fgh;fgh;jsdfdsflknljsdf",99);
    for(int i=0;i<100;i++){
        mq_send(mq,(char *)  &msg, MAX_SIZE, 0); //send message
        printf("Sent message is : %s \n", msg);
        usleep(5000);
    }
    std::cout << "Done Sending !" << std::endl;
    if ((mqd_t) -1 == mq_close(mq)) {
        perror(QUEUE_NAME);
        exit(EXIT_FAILURE);
    }
    return 0;
}


int main(int argc, char **argv) {  
 
    std::cout << "Initilized correctly !" << std::endl;

    std::thread test1(Receive);    
    std::thread test(play);
    test.join();
    stop = true ;
 
    return 0;
}
