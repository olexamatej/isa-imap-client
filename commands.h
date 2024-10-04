#pragma once
#include <string>
#include <iostream>
#include <cstdint>


class Commands {
public:
    std::string login(uint8_t tag, std::string& username, const std::string& password);
    std::string logout(uint8_t tag);
    std::string list(uint8_t tag, const std::string& reference, const std::string& mailbox);
    std::string authenticate(uint8_t tag, const std::string& mechanism, const std::string& initial_response);
    std::string select(uint8_t tag, const std::string& mailbox);
    std::string fetch_header_important(uint8_t tag, const std::string& mailbox, const int message_id);
    std::string fetch_header(uint8_t tag, const std::string& mailbox, const int message_id);
    std::string fetch_body(uint8_t tag, const std::string& mailbox, const int message_id);
    std::string fetch(uint8_t tag, int message_id);
    std::string get_new_messages(uint8_t tag);
    
};