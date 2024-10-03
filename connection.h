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
            std::cout << "Messages new: " << only_message_headers << std::endl;
            std::cout << "Messages heads: " << only_message_headers << std::endl;
            std::cout << "Inbox: " << inbox << std::endl;
        }

        std::string auth_file;
        std::string out_dir;

        bool encryption;
        std::string cert_dir = "/etc/ssl/certs";
        std::string cert_file;
        bool only_new_messages;
        bool only_message_headers;
        std::string inbox = "INBOX";

        std::string user_name = "";
        std::string user_password = "";
        std::string server = "imap.centrum.sk";
        std::string port;
};