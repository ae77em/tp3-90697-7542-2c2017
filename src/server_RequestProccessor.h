#ifndef SERVER_REQUESTPROCCESSOR_H
#define SERVER_REQUESTPROCCESSOR_H

#include "common_Thread.h"
#include "common_Socket.h"
#include "common_Command.h"

class RequestProccessor : public Thread {
private:
    char op;
    Socket &client;
    Command *command;
    
public:
    RequestProccessor(char op, Socket &client, Command *cmd);
    virtual ~RequestProccessor();
    
    void run();
    
private:
    RequestProccessor() = delete;
    RequestProccessor(const RequestProccessor& orig) = delete;
};

#endif /* SERVER_REQUESTPROCCESSOR_H */

