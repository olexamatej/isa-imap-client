#include "commands.h"

#define EOL "\r\n"
#define SPACE " "



std::string Commands::login(uint8_t tag, std::string& username, const std::string& password) {
    std::string command = std::to_string(tag) + " LOGIN " + username + SPACE + password + EOL;
    return command;
}

std::string Commands::authenticate(uint8_t tag, const std::string& mechanism, const std::string& initial_response) {
    std::string command = std::to_string(tag) + " AUTHENTICATE " + mechanism + SPACE + initial_response + EOL;
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

std::string Commands::fetch_all(uint8_t tag) {
    std::string command = std::to_string(tag) + " FETCH 1:* BODY[]\r\n";
    return command;
}

std::string Commands::select(uint8_t tag, const std::string& mailbox) {
    std::string command = std::to_string(tag) + " SELECT " + mailbox + "\r\n";
    return command;
}