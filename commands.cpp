#include "commands.h"

#define EOL "\r\n"
#define SPACE " "
std::string Commands::login(uint8_t tag, std::string& username, const std::string& password) {
    std::string command = std::to_string(tag) + " LOGIN " + username + SPACE + password + EOL;
    return command;
}

std::string Commands::logout(uint8_t tag){
    std::string command = std::to_string(tag) + " LOGOUT\r\n";
    return command;
}

std::string Commands::list(uint8_t tag, const std::string& reference, const std::string& mailbox) {
    std::string command = std::to_string(tag) + " LIST " + reference + SPACE + mailbox + EOL;
    return command;
}