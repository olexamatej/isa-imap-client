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
            conn->user_name = line.substr(11);
        }
        else
        {
            std::cout << "Error reading login" << std::endl;
            exit(1);
        }

        std::getline(file, line);
        if (line.substr(0, 8) == "password" && line[9] == '=' && line.size() > 11)
        {
            conn->user_password = line.substr(11);
        }
        else
        {
            std::cout << "Error reading password" << std::endl;
            exit(1);
        }
    }
    else
    {
        std::cout << "Error opening file" << std::endl;
    }

    file.close();
}

void File_manager::save_mail(std::string file_name, std::string mail, std::string out_dir)
{
    std::ofstream file(out_dir + "/" + file_name);
    if (file.is_open())
    {
        file << mail;
    }
    else
    {   
        std::cout << "Error opening file" << std::endl;
    }
    file.close();
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