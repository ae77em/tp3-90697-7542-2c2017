#include "server_ShuttingDownHandler.h"

#include <iostream>
#include <string.h>

ShuttingDownHandler::ShuttingDownHandler() { }

ShuttingDownHandler::~ShuttingDownHandler() { }

void ShuttingDownHandler::run(){
    while(true){
        if(std::cin.get() == 'q') {
            std::cout << "ENTRO UNA Q..." << std::endl;
            is_ended = true;
            break;
        }
    }
}

bool ShuttingDownHandler::has_ended(){
    return is_ended;
}
