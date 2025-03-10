// Author: Matej Olexa (xolexa03)

#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "connection.h"


class File_manager {

public:
    void get_auth_data(Connection* conn, std::string file_name);
    void save_mail(std::string file_name, std::string mail, std::string out_dir_);
    bool check_existence(std::string directory, std::string file_name, bool headers_only);
    void remove_file(std::string file_name);

};
