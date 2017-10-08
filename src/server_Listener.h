#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include "common_Socket.h"
#include "common_Operation.h"
#include "server_ClientRequestHandler.h"

#include <vector>
#include <cstdlib>

class Listener : public Thread {
private:
    Socket server;
    std::vector<ClientRequestHandler*> threads;
    std::vector<Socket*> clients;
    
public:
    Listener(uint16_t port);
    Listener(const Listener& orig);
    virtual ~Listener();
    
    void run();
    
    void shutdown();
    
private:
    Listener() = delete;
};

#endif /* SERVER_HANDLER_H */

