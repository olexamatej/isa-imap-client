#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>


class Client {
    public:
        Client(std::string ip_address, std::string port, bool encryption);
        ~Client();
        void send(std::string message);
        void connect();
        std::string receive();
        void receiveTimeout(int){};
        void init_openssl();
        bool verify_certificate();
    private:
        int _socket;
        std::string ip_address;
        std::string port;
        bool encryption;

        SSL_CTX* ctx;
        SSL* ssl;

};


#endif