#include "tcp_client.h"


//creating connection
TCPClient::TCPClient(std::string ip_address, std::string port) {
    this->ip_address = ip_address;
    this->port = port;
    connect();
}

TCPClient::~TCPClient() {
    close(_socket);
}


void TCPClient::connect() {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    std::memset(&hints, 0, sizeof hints);
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    //get ip from domain
    if ((status = getaddrinfo(ip_address.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    _socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (_socket == -1) {
        perror("socket");
        exit(1);
    }

    //creating connection
        if (::connect(_socket, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        if (errno != EINPROGRESS) {
            perror("connect");
            close(_socket);
            exit(1);
        }
    }

    freeaddrinfo(servinfo);
}
//send message
void TCPClient::send(std::string message) {
    ssize_t bytes_sent = ::send(_socket, message.c_str(), message.size(), 0);
    if (bytes_sent == -1) {
        perror("send");
    }
}

//receive message
std::string TCPClient::receive(){
    char buffer[2000];
    ssize_t bytes_received = recv(_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("recv");
            }
        // Return empty string to indicate no data available
        return "";
    }
    // std::cout << "Received " << bytes_received << " bytes from " << ip_address << ":" << port << std::endl;
    return std::string(buffer, bytes_received);
}


// int main() {
//     std::string ip_address = "127.0.0.1";
//     std::string port = "5553";
    
//     // TCPClient client(ip_address, port);
//     TCPClient client(ip_address, port);
//     client.send("Mam rad vlaky\n");

//     std::cout << "conntected \n";
//    while(1) {
//         std::string message = client.receive();
//         if (message.empty()) {
//             break;
//         }
//         std::cout << message << std::endl;
//     }

//     return 0;
// }