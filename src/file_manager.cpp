#include "file_manager.h"


void File_manager::get_auth_data(Connection* conn, std::string file_name)
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
            std::cout << "Error reading login" << std::endl;
            exit(1);
        }

        std::getline(file, line);
        if (line.substr(0, 8) == "password" && line[9] == '=' && line.size() > 11)
        {
            conn->user_password_ = line.substr(11);
        }
        else
        {
            std::cout << "Error reading password" << std::endl;
            exit(1);
        }
    }
    else
    {
        std::cout << "Error opening file when getting data" << std::endl;
    }

    file.close();
}

//TODO here is error on auto tests
void File_manager::save_mail(std::string file_name, std::string mail, std::string out_dir_)
{
    std::ofstream file(out_dir_ + "/" + file_name);
    if(file_name.length() > 256){
        std::cout << "Error: Could not get file name" << std::endl;
    }
    else if(file_name.find('/') != std::string::npos){
        std::cout << "Error: Invalid file name" << std::endl;
    }

    else{
    if (file.is_open())
    {   
        size_t first_newline = mail.find('\n');
        
        // find last parenthese, that is where the mail ends
        size_t last_paren = mail.rfind(')');
        
        size_t last_newline = mail.rfind('\n', last_paren);
        
        if (first_newline != std::string::npos && last_newline != std::string::npos && first_newline < last_newline)
        {
            std::string email_content = mail.substr(first_newline + 1, last_newline - first_newline - 3);
            file << email_content;
        }
        else
        {
            file << mail;
        }
    }
    else
    {   
        std::cout << "Error opening file" << std::endl;
    }
    file.close();
    }
}

bool File_manager::check_existence(std::string file_name)
{
    std::ifstream file(file_name);
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

void File_manager::remove_file(std::string file_name)
{
    if (remove(file_name.c_str()) != 0)
    {
        std::cout << "Error deleting file" << std::endl;
    }
}