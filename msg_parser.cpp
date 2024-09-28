#include "msg_parser.h"
#include <iostream>

void MsgParser::get_message_count(const std::string response) {
    std::string::size_type pos = response.find("EXISTS");
    if (pos != std::string::npos) {
        this->existing_messages = response[2];
    }   
}

void MsgParser::get_new_message_count(const std::string response) {
    std::string::size_type pos = response.find("RECENT");
    if (pos != std::string::npos) {
        this->new_messages = response[2];
    }
}

void MsgParser::get_capability(const std::string response) {
    std::string::size_type pos = response.find("CAPABILITY");
    std::string capability;

    if (pos != std::string::npos) {
        pos += 10; // Move past the word "CAPABILITY" (9 chars + space)
        
        while (pos < response.length()) {
            // Skip leading spaces
            while (pos < response.length() && response[pos] == ' ') {
                pos++;
            }

            if (response[pos] == '\r' || response[pos] == '\n') {
                break;
            }

            // Capture the capability until we hit a space or line ending
            std::string::size_type start_pos = pos;
            while (pos < response.length() && response[pos] != ' ' && response[pos] != '\r' && response[pos] != '\n') {
                pos++;
            }

            // Ensure we have a valid capability to push
            if (start_pos != pos) {
                this->capabilities.push_back(response.substr(start_pos, pos - start_pos));
            }
        }
    }
}

void MsgParser::get_mailbox_names(const std::string response) {
    std::string::size_type pos = 0;
    std::string mailbox;

    while ((pos = response.find("\"/\"", pos)) != std::string::npos) {
        pos += 3; 
        mailbox.clear(); 
        while (pos < response.size() && response[pos] != '\n' && response[pos] != '\r') {
            mailbox += response[pos];
            pos++;
        }

        this->mailbox_list.push_back(mailbox);

        pos+=2;     //next line
    }
}