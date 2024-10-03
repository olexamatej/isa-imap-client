#include <iostream>
#include <string>
#include "runner.h"
#include "arg_parser.h"
#include "file_manager.h"
#include "connection.h"

int main(int argc, char *argv[])
{   
    Connection conn = parse_arg(argc, argv);
    File_manager fm = File_manager();
    fm.get_auth_data(&conn, conn.auth_file);
    Runner runner(conn, fm);

    

    return 0;
}