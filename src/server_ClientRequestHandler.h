#ifndef SERVER_REQUESTPROCCESSOR_H
#define SERVER_REQUESTPROCCESSOR_H

#include "common_Thread.h"
#include "common_Socket.h"
#include "common_Operation.h"

class ClientRequestHandler : public Thread {
private:
    char op;
    Socket &client;
    Operation command;
    
public:
    ClientRequestHandler(Socket &client);
    virtual ~ClientRequestHandler();
    
    void run();
    
private:
    ClientRequestHandler() = delete;
    ClientRequestHandler(const ClientRequestHandler& orig) = delete;
};

#endif /* SERVER_REQUESTPROCCESSOR_H */

