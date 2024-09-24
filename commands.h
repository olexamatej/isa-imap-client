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
    
};