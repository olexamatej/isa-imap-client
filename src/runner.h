// Author: Matej Olexa (xolexa03)

#pragma once

#include "connection.h"
#include "file_manager.h"
#include "client.h"
#include "commands.h"
#include "msg_parser.h"
#include <algorithm>

class Runner {
public:
    Runner(Connection conn, File_manager file_manager);
    void run();
    bool initialize_connection();
private:
    bool send_and_receive(const std::string command, std::string &response);
    bool fetch_new_messages(std::vector<int> new_messages);
    bool process_single_message(int msg_id, bool headers_only);
    bool process_messages(const std::vector<int> messages, bool headers_only);
    
    Connection conn_;
    File_manager file_manager_;
    Client client_;
    Commands commands_;
    MsgParser parser_;
    uint64_t tag_ = 120;
};
