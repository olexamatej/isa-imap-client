// Author: Matej Olexa (xolexa03)

#pragma once
#include <string>
#include <iostream>
#include <cstdint>

class Commands {
public:
    std::string login(uint64_t tag, std::string username, std::string password);
    std::string logout(uint64_t tag);
    std::string list(uint64_t tag, std::string reference, std::string mailbox);
    std::string select(uint64_t tag, std::string mailbox);
    std::string fetch_header_important(uint64_t tag, int message_id);
    std::string fetch_header(uint64_t tag, int message_id);
    std::string fetch(uint64_t tag, int message_id);
    std::string get_new_messages(uint64_t tag);
};