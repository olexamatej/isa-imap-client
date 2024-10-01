#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "connection.h"


class File_manager {

public:
    void get_auth_data(Connection* conn, std::string file_name);
    void save_mail(std::string file_name, std::string mail, std::string out_dir);
    bool check_existence(std::string file_name);

};
