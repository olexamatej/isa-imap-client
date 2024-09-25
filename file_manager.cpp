#include "file_manager.h"

void File_manager::get_auth_data()
{
    std::string login;
    std::string password;

    std::string line;
    std::ifstream file("auth_file");

    if (file.is_open())
    {
        std::getline(file, line);
        if (line.substr(0, 8) == "username" && line[9] == '=' && line.size() > 11)
        {
            login = line.substr(11);
        }
        else
        {
            std::cout << "Error reading login" << std::endl;
            exit(1);
        }

        std::getline(file, line);
        if (line.substr(0, 8) == "password" && line[9] == '=' && line.size() > 11)
        {
            password = line.substr(11);
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