// Author: Matej Olexa (xolexa03)

#include "arg_parser.h"

// parses arguments and returns Connection object with saved values
Connection parse_arg(int argc, char *argv[])
{
    Connection conn;
    int opt;
    bool cert_set = false;
    opterr = 0;
    // parse command line arguments
    while ((opt = getopt(argc, argv, "p:Tc:C:nha:b:o:i")) != -1)
    {
        switch (opt)
        {
        case 'p':
            if (optarg == NULL)
            {
                std::cerr << "ERROR: -p requires a numeric argument\n";
                exit(EXIT_FAILURE);
            }
            conn.port_ = optarg;
            break;
        case 'i':
            conn.interactive_ = true;
            break;
            
        case 'T':
            conn.encryption_ = true;
            break;
        case 'c':
            conn.cert_file_ = optarg;
            cert_set = true;
            break;
        case 'C':
            conn.cert_dir_ = optarg;
            cert_set = true;
            break;
        case 'n':
            conn.only_new_messages_ = true;
            break;
        case 'h':
            conn.only_message_headers_ = true;
            break;
        case 'a':
            conn.auth_file_ = optarg;
            break;
        case 'b':
            conn.inbox_ = optarg;
            break;
        case 'o':
            conn.out_dir_ = optarg;
            break;
        default:
            std::cerr << "Unknown option: " << opt << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // After processing options, set conn.server_ to the next argument
    if(conn.auth_file_.empty() || conn.out_dir_.empty()){
        std::cerr << "ERROR: Auth file and out dir are required\n";
        exit(EXIT_FAILURE);
    }
    // check any invalid arguments
    if (optind < argc)
    {
        conn.server_ = argv[optind];
    }
    else
    {
        std::cerr << "ERROR: No server_ address provided\n";
        exit(EXIT_FAILURE);
    }
    if (conn.port_.empty())
    {
        if (conn.encryption_)
        {
            conn.port_ = "993";
        }
        else
        {
            conn.port_ = "143";
        }
    }
    if(cert_set && conn.encryption_ == false){
        
        std::cerr << "ERROR: Certificate file or dir set, but encryption (-T) is not enabled\n";
        exit(EXIT_FAILURE);
    }
    

    return conn;
}