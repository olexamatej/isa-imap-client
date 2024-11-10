#include "arg_parser.h"

Connection parse_arg(int argc, char *argv[])
{
    Connection conn;
    int opt;
    bool cert_set = false;
    opterr = 0;
    // parse command line arguments, it checks long and short versions
    while ((opt = getopt(argc, argv, "p:Tc:C:nha:b:o:i")) != -1)
    {
        switch (opt)
        {
            // port_
        case 'p':
            if (optarg == NULL)
            {
                std::cerr << "Error: -p requires a numeric argument\n";
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
            cert_set;
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
        std::cerr << "Error: Auth file and out dir are required\n";
        exit(EXIT_FAILURE);
    }
    if (optind < argc)
    {
        conn.server_ = argv[optind];
    }
    else
    {
        std::cerr << "Error: No server_ address provided\n";
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
        std::cerr << "WARNING: Certificate file set, but encryption_ is not enabled\n";
    }
    

    return conn;
}