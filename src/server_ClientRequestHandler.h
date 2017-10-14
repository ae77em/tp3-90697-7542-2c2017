#ifndef SERVER_CLIENT_REQUEST_HANDLER_H
#define SERVER_CLIENT_REQUEST_HANDLER_H

#include "common_Thread.h"
#include "common_Socket.h"
#include "common_Command.h"

class ClientRequestHandler : public Thread {
private:
    char op;
    Socket &client;
    Command command;
    
public:
    explicit ClientRequestHandler(Socket &client);
    virtual ~ClientRequestHandler();
    
    void run();
    
private:
    ClientRequestHandler() = delete;
    ClientRequestHandler(const ClientRequestHandler& orig) = delete;
};

#endif /* SERVER_REQUESTPROCCESSOR_H */

