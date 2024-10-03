#include "runner.h"


Runner::Runner(Connection conn, File_manager file_manager){ 
    Client client(conn.server, conn.port, conn.encryption);
    if(conn.encryption){
        if(!client.verify_certificate()){
            std::cout << "Certificate verification failed" << std::endl;
            exit(1);
        }
    }

    Commands commands = Commands();
    MsgParser parser = MsgParser();

    client.receive(tag);

    client.send(commands.login(tag++, conn.user_name, conn.user_password));
    parser.get_capability(client.receive(tag));
    client.send(commands.select(tag++, "INBOX"));
    std::string response = client.receive(tag);
    parser.get_message_count(response);
    parser.get_new_message_count(response);

    // for(int i = 1; i <= parser.message_count; i++){
    client.send(commands.fetch_header_important(tag++, "INBOX", 4));
    response = client.receive(tag);
    
    std::string file_name = parser.get_file_name(response);

    client.send(commands.fetch(tag++, 4));
    response = client.receive(tag);
    std::cout << response << std::endl;

    
    // std::cout << response << std::endl;
    file_manager.save_mail(file_name, response, conn.out_dir);
}