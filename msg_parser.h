#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <tuple>


class MsgParser {
public:
    void get_message_count(const std::string response);
    std::vector<int> get_new_messages(const std::string response);
    void get_capability(const std::string response);
    void get_mailbox_names(const std::string response);
    std::string get_mailbox_header(const std::string reponse);

    std::string extract_header_field(const std::string header, const std::string field);
    std::string get_file_name(const std::string response);

    std::vector<std::string> capabilities_;
    std::vector<std::string> mailbox_list_;
    int message_count_ = 0;
    int new_messages_;

};  