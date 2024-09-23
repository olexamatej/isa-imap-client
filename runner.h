#pragma once


#include "tcp_client.h"

class Runner {
    public:
        Runner(std::string user_name, std::string user_password, std::string server);
    private:
        std::string user_name;
        std::string user_password;
        std::string server;

};