#include "common_Socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <iostream>
#include <string>

Socket::Socket() {
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (this->socket == -1) {
        throw std::string("No se pudo abrir el socket.");
    }
}

Socket::Socket(int socket) {
    this->socket = socket;
}

Socket::~Socket() {
    shutdown();
    close();
}

void Socket::bind_and_listen(unsigned short port) {
    int bind_result = 0;
    struct sockaddr_in addr;

    memset((char *) &addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind_result = ::bind(this->socket,
            (struct sockaddr *) &addr,
            (socklen_t)sizeof (struct sockaddr));

    if (bind_result < 0) {
        throw std::string("Error al realizar el bind.");
    } else {
        int listen_status = ::listen(this->socket, 1);

        if (listen_status < 0) {
            throw std::string("Error al realizar el listen.");
        }
    }
}

void Socket::connect(const char* host_name, unsigned short port) {
    struct hostent *he;

    if ((he = ::gethostbyname(host_name)) == NULL) {
        throw std::string("Error al obtener el host.");
    } else {
        struct sockaddr_in their_addr;
        int conn_status;

        their_addr.sin_family = AF_INET; // orden de bytes de la máquina
        their_addr.sin_port = htons(port); // short, orden de bytes de la red
        their_addr.sin_addr = *((struct in_addr *) he->h_addr_list[0]);
        memset(&(their_addr.sin_zero), 8, sizeof (int)); // poner a 0 el resto

        conn_status = ::connect(
                this->socket,
                (struct sockaddr *) &their_addr,
                sizeof (struct sockaddr));

        if (conn_status == -1) {
            throw std::string("Error al realizar el connect.");
        }
    }
}

void Socket::accept(Socket &accepted_socket) {
    unsigned int newsockfd;

    newsockfd = ::accept(this->socket, NULL, NULL);

    if (newsockfd < 0) {
        throw std::string("Error al realizar el accept.");
    } else {
        accepted_socket.set_socket(newsockfd);
    }
}

void Socket::send(const char* buffer, size_t length) {
    int sent = 0;
    int s = 0;
    bool is_open_socket = true;

    while (sent < (int)length && is_open_socket) {
        s = ::send(this->socket,
                &buffer[sent],
                length - sent,
                MSG_NOSIGNAL);

        if (s == 0) {
            is_open_socket = false;
        } else if (s < 0) {
            throw std::string("Error al realizar el send.");
        } else {
            sent += s;
        }
    }
}

void Socket::receive(char* buffer, size_t length) {
    int received = 0;
    int r = 0;
    bool is_open_socket = true;

    while (received < (int)length && is_open_socket) {
        r = ::recv(this->socket,
                &buffer[received],
                length - received,
                MSG_NOSIGNAL);

        if (r == 0) {
            is_open_socket = false;
        } else if (r < 0) {
            throw std::string("Error al realizar el receive.");
        } else {
            received += r;
        }
    }
}

void Socket::shutdown() {
    ::shutdown(this->socket, SHUT_RDWR);
}

void Socket::close(){
    ::close(this->socket);
}

void Socket::shutdown_send() {
    ::shutdown(this->socket, SHUT_WR);
}

int Socket::get_socket(){
    return this->socket;
}

int Socket::set_socket(int s){
    this->socket = s;
    return this->socket;
}
