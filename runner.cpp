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

    client.send(commands.login(tag++, conn.user_name, conn.user_password));
    std::cout << client.receive() << std::endl;
    // Select the INBOX mailbox
    client.send(commands.select(tag++, "INBOX"));
    std::cout << client.receive() << std::endl;
    

    // Fetch all messages
    client.send(commands.fetch_all(tag++));
        std::cout << client.receive() << std::endl;

    // Logout
    client.send(commands.logout(tag++));
    std::cout << client.receive() << std::endl;
}