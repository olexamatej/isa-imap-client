#include "runner.h"

Runner::Runner(Connection conn, File_manager file_manager) 
    : conn_(conn)
    , file_manager_(file_manager)
    , client_(conn.server_, conn.port_, conn.encryption_, conn.cert_file_, conn.cert_dir_)
    , commands_()
    , parser_()
    , tag_(1) 
{
    if (conn_.encryption_ && !client_.verify_certificate())
    {
        std::cout << "Certificate verification failed" << std::endl;
        exit(1);
    }
}


bool Runner::send_and_receive(const std::string& command, std::string& response) {
    client_.send(command);
    response = client_.receive(tag_);
    tag_++;
    return !response.empty();
}

bool Runner::initialize_connection() {
    std::string response;
    if (!send_and_receive("", response)) {
        std::cout << "Error: Initial connection failed" << std::endl;
        return false;
    }
    std::cout << response;

    if (!send_and_receive(commands_.login(tag_, conn_.user_name_, conn_.user_password_), response)) {
        std::cout << "Error: Login failed" << std::endl;
        return false;
    }
    parser_.get_capability(response);

    if (!send_and_receive(commands_.select(tag_, conn_.inbox_), response)) {
        std::cout << "Error: Select inbox failed" << std::endl;
        return false;
    }
    parser_.get_message_count(response);
    std::cout << "Message count: " << parser_.message_count_ << std::endl;
    
    return true;
}

bool Runner::fetch_new_messages(std::vector<int>& new_messages) {
    std::string response;
    if (!send_and_receive(commands_.get_new_messages(tag_), response)) {
        std::cout << "Error: Failed to get new messages" << std::endl;
        return false;
    }
    new_messages = parser_.get_new_messages(response);
    std::cout << "New messages: " << new_messages.size() << std::endl;
    return true;
}

bool Runner::process_single_message(int msg_id, bool headers_only) {
    std::string response;
    if (!send_and_receive(commands_.fetch_header_important(tag_, msg_id), response)) {
        std::cout << "Error: Failed to fetch header for message " << msg_id << std::endl;
        return false;
    }

    std::string file_name = parser_.get_file_name(response);
    if (file_manager_.check_existence(conn_.out_dir_ + "/" + file_name)) {
        return true;  // Skip existing files
    }

    if (headers_only) {
        if (!send_and_receive(commands_.fetch_header(tag_, msg_id), response)) {
            std::cout << "Error: Failed to fetch full header for message " << msg_id << std::endl;
            return false;
        }
        file_manager_.save_mail("H-" + file_name, response, conn_.out_dir_);
    } else {
        if (!send_and_receive(commands_.fetch(tag_, msg_id), response)) {
            std::cout << "Error: Failed to fetch message " << msg_id << std::endl;
            return false;
        }
        if (file_manager_.check_existence(conn_.out_dir_ + "/" + "H-" + file_name)) {
            file_manager_.remove_file(conn_.out_dir_ + "/" + "H-" + file_name);
        }
        file_manager_.save_mail(file_name, response, conn_.out_dir_);
    }
    
    return true;
}

void Runner::process_messages(const std::vector<int>& messages, bool headers_only) {
    for (int msg_id : messages) {
        if (!process_single_message(msg_id, headers_only)) {
            std::cout << "Skipping message " << msg_id << " due to errors" << std::endl;
        }
    }
}

void Runner::run() {
    if (!initialize_connection()) {
        return;
    }

    std::vector<int> messages_to_process;
    
    if (conn_.only_new_messages_) {
        if (!fetch_new_messages(messages_to_process)) {
            return;
        }
    } else {
        // Process all messages
        messages_to_process.reserve(parser_.message_count_);
        for (int i = 1; i <= parser_.message_count_; i++) {
            messages_to_process.push_back(i);
        }
    }

    process_messages(messages_to_process, conn_.only_message_headers_);

    std::string response;
    if (!send_and_receive(commands_.logout(tag_), response)) {
        std::cout << "Error: Logout failed" << std::endl;
        return;
    }
    std::cout << "Logged out" << std::endl;
}

