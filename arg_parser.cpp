#include "arg_parser.h"

Connection parse_arg(int argc, char *argv[])
{
    Connection conn;
    int opt;
    bool cert_set = false;
    opterr = 0;
    // parse command line arguments, it checks long and short versions
    while ((opt = getopt(argc, argv, "p:Tc:C:nha:b:o:")) != -1)
    {
        switch (opt)
        {
            // port
        case 'p':
            if (optarg == NULL)
            {
                std::cerr << "Error: -p requires a numeric argument\n";
                exit(EXIT_FAILURE);
            }
            conn.port = optarg;
            break;

        case 'T':
            conn.encryption = true;
            break;
        case 'c':
            conn.cert_file = optarg;
            cert_set = true;
            break;
        case 'C':
            conn.cert_dir = optarg;
            cert_set;
            break;
        case 'n':
            conn.only_new_messages = true;
            break;
        case 'h':
            conn.only_message_headers = true;
            break;
        case 'a':
            conn.auth_file = optarg;
            break;
        case 'b':
            conn.inbox = optarg;
            break;
        case 'o':
            conn.out_dir = optarg;
            break;
        default:
            std::cerr << "Unknown option: " << opt << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // After processing options, set conn.server to the next argument
    if (optind < argc)
    {
        conn.server = argv[optind];
    }
    else
    {
        std::cerr << "Error: server argument is required\n";
        exit(EXIT_FAILURE);
    }
    if (conn.port.empty())
    {
        if (conn.encryption)
        {
            conn.port = "993";
        }
        else
        {
            conn.port = "143";
        }
    }
    if(cert_set && conn.encryption == false){
        std::cerr << "WARNING: Certificate file set, but encryption is not enabled\n";
    }
    

    return conn;
}