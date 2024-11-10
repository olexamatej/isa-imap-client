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
        Client(std::string ip_address, std::string port_, bool encryption_, std::string cert_file_, std::string cert_dir_);
        ~Client();
        void send(std::string message);
        void connect();
        std::string receive(int tag);
        void receiveTimeout(int){};
        void init_openssl(std::string cert_file_, std::string cert_dir_);
        bool verify_certificate();
    private:
        int _socket;
        std::string ip_address_;
        std::string port_;
        bool encryption_;

        SSL_CTX* ctx;
        SSL* ssl;

};


#endif