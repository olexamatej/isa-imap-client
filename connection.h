#pragma once

#include <string>
#include <iostream>

// Connection class, used for storing connection parameters
class Connection{
    public:
        Connection();
        void print_connection(){
            std::cout << "server_: " << server_ << std::endl;
            std::cout << "port_: " << port_ << std::endl;
            std::cout << "SSL: " << encryption_ << std::endl;
            std::cout << "Cert dir: " << cert_dir_ << std::endl;
            std::cout << "Cert file: " << cert_file_ << std::endl;
            std::cout << "Auth file: " << auth_file_ << std::endl;
            std::cout << "Out dir: " << out_dir_ << std::endl;
            std::cout << "Messages new: " << only_message_headers_ << std::endl;
            std::cout << "Messages heads: " << only_message_headers_ << std::endl;
            std::cout << "inbox: " << inbox_ << std::endl;
        }

        std::string auth_file_;
        std::string out_dir_;
        bool interactive_ = false;
        bool encryption_;
        std::string cert_dir_ = "/etc/ssl/certs";
        std::string cert_file_;
        bool only_new_messages_;
        bool only_message_headers_;
        std::string inbox_ = "inbox";

        std::string user_name_ = "";
        std::string user_password_ = "";
        std::string server_;
        std::string port_;
};