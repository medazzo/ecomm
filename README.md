# ecomm

[![Build Status](https://travis-ci.org/medazzo/ecomm.svg?branch=master)](https://travis-ci.org/medazzo/ecomm)

Full Duplex IPC Communication olibrary based on Named pipes or System V Message Queue.

This library can work in one direction : one process will read and the others will write or in both : full duplex : both can read and write.

The Library will manipulates strnig and object that can be serialized into string and deserialized from string.

It work by defaults using Named pipes FIFO, and also can use V system  Messages Queue.

## ecomm concept

On every half duplex communication required , it create a pipe(or a Queue ) and manage sending receiving  by proposnig the correct Api .
That's managed by class *EasinComm* .
So the class *EasinCommBi* that support Full Duplex haas two instace of  *EasinComm* .
The library doesnt not manage thead , it's no the caller responsaability.


# ecomm Object 

We call that Object that be sent or reveic in communication between process .
It can be any class that fit soe condition :
* have a cstructor from string , that ca ndeserialize object from that params
* implement serialize from *src/EasinCommObj.h*
```
class EasinCommObj {
public:
    virtual std::string Serialize() = 0 ;
    
};
```

## How ecomm Object  is Sent/Received

The serialization operation must be transorm objet params in a string where :
- fields will be separated by *;*
- once done , the result must be end with *END;*

Theses two constants, are defined in *src/EasinComm.h*, and can be changed if needed :
```
#define END_STRING          "END;"
#define END_STRING_LENGTH   4
#define SEP_STRING          ";"
```

The deserialization operation shall do the oposite.

An example to follw is here  *tests/EasinCommand.f*
```
class EasinCommand: public EasinCommObj {
public:
...
    EasinCommand(std::string command)
    {
        std::istringstream f(command);
        std::string s;  
        int i=0;
        while (getline(f, s, ';')) {
            if(i == 0)
                m_id = s;
            else if(i==1)
                m_command = s ;
            else
                m_params.push_back(s);
            i++;
        }
    };

    std::string Serialize(){
        std::string  params="";
        for (int i=0;i<m_params.size();i++)
            params+= m_params[i] + SEP_STRING;
        return std::string(m_id + SEP_STRING + m_command + SEP_STRING + params + END_STRING);
    };

private:
    std::string m_id;
    std::string m_command;
    std::vector<std::string> m_params;
};

```

# ecomm tests

![ecomm Tests](picts/ecomm.png?raw=true "half and full duplex tests with small amount of messages")

## half duplex test
It's built by default ( we can disable that using cmake option )




## full duplex test



