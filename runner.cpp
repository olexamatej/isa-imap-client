#include "runner.h"

Runner::Runner(std::string user_name, std::string user_password, std::string server) {
    TCPClient client(server, "143");
    std::cout << client.receive() << std::endl;

    client.send("AAAA LOGIN " + user_name + " " + user_password + "\r\n");
    std::cout << client.receive() << std::endl;


}