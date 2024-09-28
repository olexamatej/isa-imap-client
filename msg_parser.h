#pragma once

#include <string>
#include <vector>


class MsgParser {
public:
    void get_message_count(const std::string response);
    void get_new_message_count(const std::string response);
    void get_capability(const std::string response);
    void get_mailbox_names(const std::string response);

    std::vector<std::string> capabilities;
    std::vector<std::string> mailbox_list;

private:
    int existing_messages;
    int new_messages;
    std::string mailbox_name;
};  