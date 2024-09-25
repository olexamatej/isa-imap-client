#include <iostream>
#include <string>
#include "runner.h"
#include "arg_parser.h"
#include "file_manager.h"


int main(int argc, char *argv[])
{   
    Connection conn = parse_arg(argc, argv);
    File_manager fm;
    fm.get_auth_data(&conn);
    std::cout << "password is : " <<  conn.user_name;
    Runner runner(conn);
    return 0;
}