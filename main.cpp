#include <iostream>
#include <string>
#include "runner.h"
#include "arg_parser.h"


int main(int argc, char *argv[])
{   
    Connection conn = parse_arg(argc, argv);
    Runner runner(user_name, user_password, server);
    return 0;
}
