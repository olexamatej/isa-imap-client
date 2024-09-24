#include "runner.h"

Runner::Runner(std::string user_name, std::string user_password, std::string server) {
    Client client(server, "143", false);

    std::cout << client.receive() << std::endl;
    Commands commands = Commands();
    client.send(commands.login(tag, user_name, user_password));
    std::cout << client.receive() << std::endl;

    client.send(commands.list(tag, "\"\"", "*"));

    std::cout << client.receive() << std::endl;
}