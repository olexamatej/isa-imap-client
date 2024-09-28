#include "runner.h"


Runner::Runner(Connection conn) {
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
    
    client.send(commands.list(tag++, "\"\"", "*"));



    std::string response = client.receive();
    std::cout << response << std::endl;

    parser.get_mailbox_names(response);

    for(std::string names : parser.mailbox_list){
        std::cout << names ;
    }

    // parser.get_capability(response);




    //check list of mailboxes

    // client.send(commands.list(tag++, "\"\"", "*"));
    // std::cout << client.receive() << std::endl;

    // client.send(commands.select(tag++, "INBOX"));




    // std::cout << "Number of messages: " << parser.get_message_count(response) << std::endl;

    // Select the INBOX mailbox
    

    // // Fetch all messages
    // client.send(commands.fetch_all(tag++));
    //     std::cout << client.receive() << std::endl;

    // // Logout
    // client.send(commands.logout(tag++));
    // std::cout << client.receive() << std::endl;
}