#pragma once

#include <string>
#include <iostream>

// Connection class, used for storing connection parameters
class Connection{
    public:
        Connection();
        void print_connection(){
            std::cout << "Server: " << server << std::endl;
            std::cout << "Port: " << port << std::endl;
            std::cout << "SSL: " << encryption << std::endl;
            std::cout << "Cert dir: " << cert_dir << std::endl;
            std::cout << "Cert file: " << cert_file << std::endl;
            std::cout << "Auth file: " << auth_file << std::endl;
            std::cout << "Out dir: " << out_dir << std::endl;
            std::cout << "Messages new: " << messages_new << std::endl;
            std::cout << "Messages heads: " << messages_heads << std::endl;
            std::cout << "Inbox: " << inbox << std::endl;
        }

        std::string server;
        std::string auth_file;
        std::string out_dir;

        int port;
        bool encryption;
        std::string cert_dir = "/etc/ssl/certs";
        std::string cert_file;
        bool messages_new;
        bool messages_heads;
        std::string inbox;
};
