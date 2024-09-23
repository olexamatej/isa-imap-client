#pragma once

#include <string>
#include <iostream>

// Connection class, used for storing connection parameters
class Connection{
    public:
        void print_connection();

        std::string server;
        std::string auth_file;
        std::string out_dir;

        
        int port;
        bool ssl;
        std::string cert_dir = "/etc/ssl/certs";
        std::string cert_file;
        bool messages_new;
        bool messages_heads;
        std::string inbox;



       
};
