#ifndef RUNNER_H
#define RUNNER_H

#include "connection.h"
#include "file_manager.h"
#include "client.h"
#include "commands.h"
#include "msg_parser.h"

class Runner {
public:
    Runner(Connection conn, File_manager file_manager);
    void run();

private:
    std::string send_and_receive(const std::string& command);
    
    Connection conn_;
    File_manager file_manager_;
    Client client_;
    Commands commands_;
    MsgParser parser_;
    int tag_;
};

#endif // RUNNER_H