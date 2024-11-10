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

std::string Runner::send_and_receive(const std::string& command) {
    client_.send(command);
    std::string response = client_.receive(tag_);
    tag_++;
    return response;
}

void Runner::run() {
    std::cout << client_.receive(tag_);

    parser_.get_capability(send_and_receive(commands_.login(tag_, conn_.user_name_, conn_.user_password_)));

    std::string response = send_and_receive(commands_.select(tag_, conn_.inbox_));
    parser_.get_message_count(response);
    std::cout << "Message count: " << parser_.message_count_ << std::endl;

    response = send_and_receive(commands_.get_new_messages(tag_));
    std::vector<int> new_messages_ = parser_.get_new_messages(response);
    std::cout << "New messages: " << new_messages_.size() << std::endl;

    if (conn_.only_new_messages_ && conn_.only_message_headers_)
    {
        for (int i = 0; i < new_messages_.size(); i++)
        {
            response = send_and_receive(commands_.fetch_header_important(tag_, new_messages_[i]));
            std::string file_name = parser_.get_file_name(response);
            if (file_manager_.check_existence(conn_.out_dir_ + "/" + file_name))
            {
                continue;
            }

            response = send_and_receive(commands_.fetch_header(tag_, new_messages_[i]));
            file_manager_.save_mail("H-" + file_name, response, conn_.out_dir_);
        }
    }

    else if (conn_.only_new_messages_)
    {
        for (int i = 0; i < new_messages_.size(); i++)
        {
            response = send_and_receive(commands_.fetch_header_important(tag_, new_messages_[i]));
            std::string file_name = parser_.get_file_name(response);
            if (file_manager_.check_existence(conn_.out_dir_ + "/" + file_name))
            {
                continue;
            }

            response = send_and_receive(commands_.fetch(tag_, new_messages_[i]));
            file_manager_.save_mail(file_name, response, conn_.out_dir_);
        }
    }

    else if (conn_.only_message_headers_)
    {
        for (int i = 1; i <= parser_.message_count_; i++)
        {
            response = send_and_receive(commands_.fetch_header_important(tag_, i));
            std::string file_name = parser_.get_file_name(response);
            if (file_manager_.check_existence(conn_.out_dir_ + "/" + file_name))
            {
                continue;
            }

            response = send_and_receive(commands_.fetch_header(tag_, i));
            file_manager_.save_mail("H-" + file_name, response, conn_.out_dir_);
        }
    }
    else
    {
        int start = conn_.only_new_messages_ ? parser_.message_count_ - parser_.new_messages_ + 1 : 1;
        for (int i = start; i <= parser_.message_count_; i++)
        {
            response = send_and_receive(commands_.fetch_header_important(tag_, i));
            std::string file_name = parser_.get_file_name(response);

            response = send_and_receive(commands_.fetch(tag_, i));

            if (file_manager_.check_existence(conn_.out_dir_ + "/" + file_name))
            {
                continue;
            }
            else if (file_manager_.check_existence(conn_.out_dir_ + "/" + "H-" + file_name))
            {
                file_manager_.remove_file(conn_.out_dir_ + "/" + "H-" + file_name);
            }
            file_manager_.save_mail(file_name, response, conn_.out_dir_);
        }
    }

    send_and_receive(commands_.logout(tag_));
    std::cout << "Logged out" << std::endl;
}