// Author: Matej Olexa (xolexa03)

#include "file_manager.h"

// get username and password from file
void File_manager::get_auth_data(Connection *conn, std::string file_name)
{

    std::string line;
    std::ifstream file(file_name);

    if (file.is_open())
    {
        std::getline(file, line);
        if (line.substr(0, 8) == "username" && line[9] == '=' && line.size() > 11)
        {
            conn->user_name_ = line.substr(11);
        }
        else
        {
            std::cerr << "ERROR: reading login" << std::endl;
            exit(1);
        }

        std::getline(file, line);
        if (line.substr(0, 8) == "password" && line[9] == '=' && line.size() > 11)
        {
            conn->user_password_ = line.substr(11);
        }
        else
        {
            std::cerr << "ERROR: reading password" << std::endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "ERROR: opening file when trying to auth" << std::endl;
        exit(1);
    }

    file.close();
}

// save mail to file
void File_manager::save_mail(std::string file_name, std::string mail, std::string out_dir_) 
{
    std::ofstream file(out_dir_ + "/" + file_name);
    if (!file.is_open()) {
        return;
    }

    size_t size_start = mail.find("{");
    size_t size_end = mail.find("}");
    
    if (size_start != std::string::npos && size_end != std::string::npos) {
        std::string size_str = mail.substr(size_start + 1, size_end - size_start - 1);
        size_t message_size = std::stoull(size_str);
        
        size_t message_start = mail.find("\r\n", size_end) + 2;
        
        if (message_start != std::string::npos) {
            file.write(mail.c_str() + message_start, message_size);
        }
    } else {
        file << mail;
    }
    
    file.close();
}
// check if file exists
bool File_manager::check_existence(std::string directory, std::string file_name, bool headers_only)
{
    std::ifstream file(directory + "/" + file_name);

    if (file.is_open())
    {
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}
// remove file
void File_manager::remove_file(std::string file_name)
{
    if (remove(file_name.c_str()) != 0);
}