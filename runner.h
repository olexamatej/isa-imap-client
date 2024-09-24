#pragma once


#include "client.h"
#include "commands.h"


class Runner {
    public:
        Runner(std::string user_name, std::string user_password, std::string server);
    private:
        std::string user_name;
        std::string user_password;
        std::string server;
        uint8_t tag = 1;

};