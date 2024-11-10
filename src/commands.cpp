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

std::string Commands::select(uint8_t tag, const std::string& mailbox) {
    std::string command = std::to_string(tag) + " SELECT " + mailbox + "\r\n";
    return command;
}

std::string Commands::fetch_header_important(uint8_t tag, const int message_id) {
    std::string command = std::to_string(tag) + " FETCH " + std::to_string(message_id) + " (BODY.PEEK[HEADER.FIELDS (Date From Message-ID Subject)])\r\n";
    return command;
}

std::string Commands::fetch_header(uint8_t tag, const int message_id) {
    std::string command = std::to_string(tag) + " FETCH " + std::to_string(message_id) + " BODY[HEADER]\r\n";
    return command;
}

std::string Commands::fetch(uint8_t tag, const int message_id) {
    std::string command = std::to_string(tag) + " FETCH " + std::to_string(message_id) + " BODY[]\r\n";
    return command;
}

std::string Commands::get_new_messages(uint8_t tag) {
    std::string command = std::to_string(tag) + " SEARCH NEW\r\n";
    return command;
}