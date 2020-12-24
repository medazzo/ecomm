#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <condition_variable>

#ifndef Easin_COMM_OBJ_H
#define Easin_COMM_OBJ_H

class EasinCommObj {
public:
    virtual std::string Serialize() = 0 ;
    
};



#endif /* Easin_COMM_OBJ_H */

