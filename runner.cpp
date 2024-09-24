#include "runner.h"

Runner::Runner(Connection conn) {
    Client client(conn.server, conn.port, conn.encryption);


    std::cout << client.receive() << std::endl;
    Commands commands = Commands();
    client.send(commands.login(tag, conn.user_name, conn.user_password));
    std::cout << client.receive() << std::endl;

    client.send(commands.list(tag, "\"\"", "*"));

    std::cout << client.receive() << std::endl;
}