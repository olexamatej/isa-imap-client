#include "msg_parser.h"

void MsgParser::get_message_count(const std::string response)
{
    std::string::size_type pos = response.find("EXISTS");
    if (pos != std::string::npos)
    {
        this->message_count = response[2] - '0';
    }
}

void MsgParser::get_new_message_count(const std::string response)
{
    std::string::size_type pos = response.find("RECENT");
    if (pos != std::string::npos)
    {
        this->new_messages = response[2] - '0';
    }
}

void MsgParser::get_capability(const std::string response)
{
    std::string::size_type pos = response.find("CAPABILITY");
    std::string capability;

    if (pos != std::string::npos)
    {
        pos += 10; // Move past the word "CAPABILITY" (9 chars + space)

        while (pos < response.length())
        {
            // Skip leading spaces
            while (pos < response.length() && response[pos] == ' ')
            {
                pos++;
            }

            if (response[pos] == '\r' || response[pos] == '\n')
            {
                break;
            }

            // Capture the capability until we hit a space or line ending
            std::string::size_type start_pos = pos;
            while (pos < response.length() && response[pos] != ' ' && response[pos] != '\r' && response[pos] != '\n')
            {
                pos++;
            }

            // Ensure we have a valid capability to push
            if (start_pos != pos)
            {
                this->capabilities.push_back(response.substr(start_pos, pos - start_pos));
            }
        }
    }
}

void MsgParser::get_mailbox_names(const std::string response)
{
    std::string::size_type pos = 0;
    std::string mailbox;

    while ((pos = response.find("\"/\"", pos)) != std::string::npos)
    {
        pos += 3;
        mailbox.clear();
        while (pos < response.size() && response[pos] != '\n' && response[pos] != '\r')
        {
            mailbox += response[pos];
            pos++;
        }

        this->mailbox_list.push_back(mailbox);

        pos += 2; // next line
    }
}

std::string MsgParser::extract_header_field(const std::string header, const std::string field)
{
    std::string::size_type pos = header.find(field);
    if (pos != std::string::npos)
    {
        pos += field.length();
        while (pos < header.length() && header[pos] == ' ')
        {
            pos++;
        }

        std::string::size_type start_pos = pos;
        while (pos < header.length() && header[pos] != '\r' && header[pos] != '\n')
        {
            pos++;
        }

        return header.substr(start_pos, pos - start_pos);
    }

    return "";
}

// gets date, from, subject and message id
std::string MsgParser::get_file_name(std::string response)
{
    std::vector<std::string> header_info;

    std::string date = extract_header_field(response, "Date: ");

    if (!date.empty())
    {
        for (int i = 0; i < date.length(); i++)
        {
            if (date[i] == ' ')
            {
                date[i] = '_';
            }
            else if (date[i] == '+')
            {
                date = date.substr(0, i - 1);
                break;
            }
            else if(date[i] == ','){
                date = date.substr(0, i) + date.substr(i+1, date.length() - i - 1);
                date[i] = '-';
            }
        
        }
        header_info.push_back(date);
    }

    std::string from = extract_header_field(response, "From: ");
    if (!from.empty())
    {
        std::size_t start_pos = from.find("<");
        std::size_t end_pos = from.find(">");

        if (start_pos != std::string::npos && end_pos != std::string::npos && end_pos > start_pos)
        {
            from = from.substr(start_pos + 1, end_pos - start_pos - 1);
        }
        header_info.push_back(from);
    }

    std::string subject = extract_header_field(response, "Subject: ");
    if (!subject.empty())
    {
        header_info.push_back(subject);
    }

    std::string message_id = extract_header_field(response, "Message-ID: ");
    if (!message_id.empty())
    {
        if(message_id[0] == '<'){
            message_id = message_id.substr(1, message_id.length() - 2);
        }
        header_info.push_back(message_id);
    }


    std::string file_name = from + "-" + date + "-" + subject + "-" + message_id + ".eml";
    return file_name;
}