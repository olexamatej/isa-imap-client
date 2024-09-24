#include "arg_parser.h"

Connection parse_arg(int argc, char *argv[])
{
    Connection conn;
    int opt;

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
            conn.port = atoi(optarg);
            break;
    
        case 'T':
            conn.encryption= true;
            break;
        case 'c':
            conn.cert_file = optarg;
            break;
        case 'C':
            conn.cert_dir = optarg;
            break;
        case 'n':
            conn.messages_new = true;
            break;
        case 'h':
            conn.messages_heads = true;
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

    return conn;
}