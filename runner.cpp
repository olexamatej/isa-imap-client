#include "runner.h"


Runner::Runner(Connection conn, File_manager file_manager){ 
    Client client(conn.server, conn.port, conn.encryption);

    if(conn.encryption){
        if(!client.verify_certificate()){
            std::cout << "Certificate verification failed" << std::endl;
            exit(1);
        }
    }

    std::cout << client.receive() << std::endl;
    Commands commands = Commands();
    MsgParser parser = MsgParser();

    client.send(commands.login(tag++, conn.user_name, conn.user_password));
    std::cout << client.receive() << std::endl;
    client.send(commands.select(tag++, "INBOX"));

    std::cout << client.receive() << std::endl;

    client.send(commands.fetch_header(tag++, "INBOX", "2"));
    std::string response = client.receive();
    std::cout << response << std::endl;
    parser.get_file_name(response);

    client.send(commands.fetch(tag++, 2));
    response = client.receive();

    file_manager.save_mail(parser.get_file_name(response), response, conn.out_dir);

}