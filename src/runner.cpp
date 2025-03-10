// Author: Matej Olexa (xolexa03)

#include "runner.h"

Runner::Runner(Connection conn, File_manager file_manager)
    : conn_(conn), file_manager_(file_manager), client_(conn.server_, conn.port_, conn.encryption_, conn.cert_file_, conn.cert_dir_), commands_(), parser_()

{
    if (conn_.encryption_ && !client_.verify_certificate())
    {
        exit(1);
    }
}

std::string to_lowercase(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

bool Runner::send_and_receive(const std::string command, std::string &response)
{
    client_.send(command);

    auto result = client_.receive(tag_);

    response = result.first;
    bool bye = result.second;
    tag_++;
    if (bye)
    {
        return false;
    }
    return true;
}

// initializes connection, returns false if fails
bool Runner::initialize_connection()
{
    std::string response;

    // send empty command to get initial response
    if (!send_and_receive("", response))
    {
        std::cerr << "ERROR: Initial connection failed" << std::endl;
        return false;
    }
    // check if response is in correct format
    if (response.find("* OK ") == std::string::npos)
    {
        std::cerr << "ERROR: Unexpected initial response format" << std::endl;
        return false;
    }
    // send login command
    if (!send_and_receive(commands_.login(tag_, conn_.user_name_, conn_.user_password_), response))
    {
        return false;
    }

    parser_.get_capability(response);

    // select mailbox
    if (!send_and_receive(commands_.select(tag_, conn_.inbox_), response))
    {
        return false;
    }
    response = to_lowercase(response);
    // check if response is in correct format

    if (response.find(" select completed") == std::string::npos)
    {
        std::cerr << "ERROR: Unexpected SELECT response format" << std::endl;
        return false;
    }

    parser_.get_message_count(response);

    return true;
}

// fetches new messages, returns false if fails
bool Runner::fetch_new_messages(std::vector<int> new_messages)
{
    std::string response;
    if (!send_and_receive(commands_.get_new_messages(tag_), response))
    {
        return false;
    }
    new_messages = parser_.get_new_messages(response);
    return true;
}

// processes single message, returns false if fails
bool Runner::process_single_message(int msg_id, bool headers_only)
{
    std::string response;
    // fetch important info from header, this is used for naming file
    if (!send_and_receive(commands_.fetch_header_important(tag_, msg_id), response))
    {
        return false;
    }

    // Get the base filename and header filename
    std::string base_filename = parser_.get_file_name(response, false);
    std::string header_filename = "H-" + base_filename;

    // Check existing files based on what we want to download
    if (headers_only)
    {
        // skip if header file already exists
        if (file_manager_.check_existence(conn_.out_dir_, header_filename, true))
        {
            return true;
        }
        // skip if full message file already exists
        if (file_manager_.check_existence(conn_.out_dir_, base_filename, false))
        {
            return true;
        }

        // download header file
        if (!send_and_receive(commands_.fetch_header(tag_, msg_id), response))
        {
            return false;
        }
        file_manager_.save_mail(header_filename, response, conn_.out_dir_);
    }
    // full messages
    else
    {
        // skip if full message file already exists
        if (file_manager_.check_existence(conn_.out_dir_, base_filename, false))
        {
            return true;
        }

        // check and remove header file if it exists before downloading full message
        if (file_manager_.check_existence(conn_.out_dir_, header_filename, true))
        {
            std::string header_path = conn_.out_dir_ + "/" + header_filename;
            file_manager_.remove_file(header_path);
        }

        // download full message
        if (!send_and_receive(commands_.fetch(tag_, msg_id), response))
        {
            return false;
        }
        file_manager_.save_mail(base_filename, response, conn_.out_dir_);
    }

    return true;
}

// processes messages, returns false if fails
bool Runner::process_messages(const std::vector<int> messages, bool headers_only)
{
    for (int msg_id : messages)
    {
        if (!process_single_message(msg_id, headers_only))
        {
            return false;
        }
    }
    return true;
}

// main function that runs the program
void Runner::run()
{
    if (!initialize_connection())
    {
        return;
    }
    std::vector<int> messages_to_process;

    // fetch only new messages
    if (conn_.only_new_messages_)
    {
        if (!fetch_new_messages(messages_to_process))
        {
            return;
        }
    }
    else
    {
        // fetch all messages
        messages_to_process.reserve(parser_.message_count_);
        for (int i = 1; i <= parser_.message_count_; i++)
        {
            messages_to_process.push_back(i);
        }
    }

    // process all messages
    if (!process_messages(messages_to_process, conn_.only_message_headers_))
    {
        return;
    }

    std::string response;
    // logout
    if (!send_and_receive(commands_.logout(tag_), response))
    {
        std::cerr << "ERROR: Logout failed" << std::endl;
        return;
    }
    std::cout << messages_to_process.size() << " downloaded messages from " << conn_.server_ << std::endl;
}
