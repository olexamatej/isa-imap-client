#pragma once

#include "client.h"
#include "commands.h"
#include "connection.h"
#include "msg_parser.h"
#include "file_manager.h"


class Runner {
    public:
        Runner(Connection conn, File_manager file_manager);
    private:
        uint8_t tag = 1;

};