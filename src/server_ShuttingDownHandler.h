#ifndef SERVER_SHUTTINGDOWNHANDLER_H
#define SERVER_SHUTTINGDOWNHANDLER_H

#include "common_Thread.h"

class ShuttingDownHandler : public Thread {
private:
    bool is_ended = false;
    
public:
    ShuttingDownHandler();
    virtual ~ShuttingDownHandler();
    
    void run();
    bool has_ended();
    
private:
    ShuttingDownHandler(const ShuttingDownHandler& orig) = delete;

};

#endif /* SERVER_SHUTTINGDOWNHANDLER_H */

