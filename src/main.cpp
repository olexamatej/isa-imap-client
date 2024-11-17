// Author: Matej Olexa (xolexa03)

#include <iostream>
#include <string>
#include "runner.h"
#include "arg_parser.h"
#include "file_manager.h"
#include "connection.h"

int main(int argc, char *argv[])
{   
    // parse arguments
    Connection conn = parse_arg(argc, argv);
    // init file manager
    File_manager fm = File_manager();
    // get username and password from file
    fm.get_auth_data(&conn, conn.auth_file_);
    // create runner
    Runner runner(conn, fm);
    // run the program
    runner.run();

    

    return 0;
}