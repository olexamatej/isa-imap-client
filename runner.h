#pragma once


#include "client.h"
#include "commands.h"
#include "connection.h"


class Runner {
    public:
        Runner(Connection conn);
    private:
        uint8_t tag = 1;

};