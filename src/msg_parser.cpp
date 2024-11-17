// Author: Matej Olexa (xolexa03)

#include "msg_parser.h"

// Parses the response to get the number of messages in the mailbox
void MsgParser::get_message_count(const std::string response)
{
    std::string::size_type pos = response.find("exists");
    // If "exists" is found, extract the number of messages
    if (pos != std::string::npos)
    {
        std::string::size_type start = 0;
        std::string::size_type end = pos;
        while (end > start && isdigit(response[end - 2]))
        {
            end--;
        }
        std::string message_count_str = response.substr(end - 1, pos - end);

        this->message_count_ = std::stoi(message_count_str);
    }
    else
    {
        this->message_count_ = 0; // If "exists" is not found, set message_count_ to 0
    }
}

// Parses the response to get the message IDs of new messages in the mailbox
// Returns a vector of message IDs
std::vector<int> MsgParser::get_new_messages(const std::string response)
{
    std::vector<int> new_messages_;
    std::string temp;
    size_t pos = response.find("* SEARCH");

    // If "* SEARCH" is found, extract the message IDs
    if (pos != std::string::npos)
    {
        pos += 8;

        while (pos < response.length() && response[pos] != '\r' && response[pos] != '\n')
        {
            while (pos < response.length() && std::isspace(response[pos]))
            {
                pos++;
            }

            if (pos >= response.length() || response[pos] == '\r' || response[pos] == '\n')
            {
                break;
            }

            temp.clear();
            while (pos < response.length() && std::isdigit(response[pos]))
            {
                temp += response[pos];
                pos++;
            }

            if (!temp.empty())
            {
                new_messages_.push_back(std::stoi(temp));
            }

            if (pos < response.length() && !std::isspace(response[pos]))
            {
                pos++;
            }
        }
    }
    return new_messages_;
}

// Parses the response to get the capabilities of the server
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
                this->capabilities_.push_back(response.substr(start_pos, pos - start_pos));
            }
        }
    }
}

// parses the response to get the mailbox names
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

        this->mailbox_list_.push_back(mailbox);

        pos += 2; // next line
    }
}

// extracts the header field from the header
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

// gets date, from, subject and message id and creates a file name 
// returns the file name as string
std::string MsgParser::get_file_name(std::string response, bool header)
{
    std::string date = extract_header_field(response, "Date: ");
    // Replace spaces with underscores, commas with dashes and remove plus signs
    if (!date.empty())
    {
        for (long unsigned int i = 0; i < date.length(); i++)
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
            else if (date[i] == ',')
            {
                date = date.substr(0, i) + date.substr(i + 1, date.length() - i - 1);
                date[i] = '-';
            }
        }
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
    }

    std::string subject = extract_header_field(response, "Subject: ");

    std::string message_id = extract_header_field(response, "Message-Id: ");
    if (message_id.empty())
    {
        message_id = extract_header_field(response, "Message-ID: ");
    }
    if (!message_id.empty())
    {
        // Remove the brackets from the message ID
        if (message_id[0] == '<')
        {
            message_id = message_id.substr(1, message_id.length() - 2);
        }
    }

    // Remove forbidden characters from the file name
    static const std::unordered_set<char> forbiddenChars = {'/', '\\', ':', '*', '?', '"', '<', '>', '|'};
    std::string cleanedFileName;

    std::string file_name;
    // Calculate maximum length considering the prefix and extension
    const size_t max_length = 250 - 4; // 4 for ".eml"
    const std::string prefix = header ? "H-" : "";
    const size_t available_length = max_length - prefix.length();

    // First create the base filename without prefix
    std::string base_name = from + "-" + date + "-" + subject + "-" + message_id;

    // Remove forbidden characters
    for (char c : base_name)
    {
        if (forbiddenChars.count(c) == 0)
        {
            cleanedFileName += c;
        }
    }

    // Truncate if necessary
    if (cleanedFileName.length() > available_length)
    {
        cleanedFileName = cleanedFileName.substr(0, available_length);
    }

    // Add prefix and extension
    file_name = prefix + cleanedFileName + ".eml";
    return file_name;
}