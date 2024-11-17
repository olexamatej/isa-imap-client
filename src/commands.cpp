// Author: Matej Olexa (xolexa03)

#include "commands.h"

#define EOL "\r\n"
#define SPACE " "

// create login command, return command as string 
std::string Commands::login(uint64_t tag, std::string username, std::string password) {
    std::string command = std::to_string(tag) + " LOGIN " + username + SPACE + password + EOL;
    return command;
}

// create logout command, return command as string
std::string Commands::logout(uint64_t tag) {
    std::string command = std::to_string(tag) + " LOGOUT" + EOL;
    return command;
}

// create list command, return command as string
std::string Commands::list(uint64_t tag, std::string reference, std::string mailbox) {
    std::string command = std::to_string(tag) + " LIST " + reference + SPACE + mailbox + EOL;
    return command;
}

// create select command, return command as string
std::string Commands::select(uint64_t tag, std::string mailbox) {
    std::string command = std::to_string(tag) + " SELECT " + mailbox + EOL;
    return command;
}

// create fetch header important command (used for naming files), return command as string
std::string Commands::fetch_header_important(uint64_t tag, int message_id) {
    std::string command = std::to_string(tag) + " FETCH " + std::to_string(message_id) + " (BODY.PEEK[HEADER.FIELDS (Date From Message-ID Subject)])" + EOL;
    return command;
}

// create fetch header command, return command as string
std::string Commands::fetch_header(uint64_t tag, int message_id) {
    std::string command = std::to_string(tag) + " FETCH " + std::to_string(message_id) + " BODY[HEADER]" + EOL;
    return command;
}

// create fetch command, return command as string
std::string Commands::fetch(uint64_t tag, int message_id) {
    std::string command = std::to_string(tag) + " FETCH " + std::to_string(message_id) + " BODY[]" + EOL;
    return command;
}

// create get new messages command, return command as string
std::string Commands::get_new_messages(uint64_t tag) {
    std::string command = std::to_string(tag) + " SEARCH NEW" + EOL;
    return command;
}