#ifndef LISTENER_H
#define LISTENER_H

#include "common_Socket.h"
#include "common_Command.h"
#include "server_ClientRequestHandler.h"

#include <vector>
#include <cstdlib>

class Listener : public Thread {
private:
    Socket server;
    std::vector<ClientRequestHandler*> threads;
    std::vector<Socket*> clients;
    
public:
    explicit Listener(uint16_t port);
    Listener(const Listener& orig);
    virtual ~Listener();
    
    void run();
    
    void shutdown();
    
private:
    Listener() = delete;
};

#endif /* SERVER_HANDLER_H */

