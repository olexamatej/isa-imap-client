#pragma once


#include "client.h"
#include "commands.h"
#include "connection.h"
#include "msg_parser.h"


class Runner {
    public:
        Runner(Connection conn);
    private:
        uint8_t tag = 1;

};