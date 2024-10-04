#include "client.h"
#include <openssl/ssl.h>

// creating connection
Client::Client(std::string ip_address, std::string port, bool encryption, std::string cert_file, std::string cert_dir)
{
    this->ip_address = ip_address;
    this->port = port;
    this->encryption = encryption;

    if (encryption)
    {
        init_openssl(cert_file, cert_dir);
    }
    connect();
}

Client::~Client()
{
    close(_socket);
    if (encryption)
    {
        SSL_free(ssl);
        SSL_CTX_free(ctx);
    }
    EVP_cleanup();
}

void Client::init_openssl(std::string cert_file, std::string cert_dir)
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_client_method());

    const char *cert_file_ptr = cert_file.c_str() ? nullptr : cert_file.c_str();
    

    if (!SSL_CTX_load_verify_locations(ctx, cert_file_ptr, cert_dir.c_str()))
    {
        std::cerr << "Failed to load CA certificates" << std::endl;
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

bool Client::verify_certificate()
{
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (cert == nullptr)
    {
        std::cerr << "Error: No certificate provided by the server" << std::endl;
        return false;
    }

    // Verify the certificate
    long result = SSL_get_verify_result(ssl);
    if (result != X509_V_OK)
    {
        std::cerr << "Error: Certificate verification failed: " << X509_verify_cert_error_string(result) << std::endl;
        X509_free(cert);
        return false;
    }

    X509_free(cert);
    return true;
}

void Client::connect()
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    std::memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    // get ip from domain
    if ((status = getaddrinfo(ip_address.c_str(), port.c_str(), &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    _socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (_socket == -1)
    {
        perror("socket");
        exit(1);
    }

    // creating connection
    if (::connect(_socket, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        if (errno != EINPROGRESS)
        {
            perror("connect");
            close(_socket);
            exit(1);
        }
    }

    // encryption

    if (encryption == true)
    {
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, _socket);
        if (SSL_connect(ssl) == -1)
        {
            ERR_print_errors_fp(stderr);
            exit(1);
        }
    }

    freeaddrinfo(servinfo);
}

// send message
void Client::send(std::string message)
{
    if (encryption == true)
    {
        SSL_write(ssl, message.c_str(), message.size());
    }
    else
    {
        ssize_t bytes_sent = ::send(_socket, message.c_str(), message.size(), 0);
        if (bytes_sent == -1)
        {
            perror("send");
        }
    }
}

// receive message
std::string Client::receive(int tag)
{
    tag = tag - 1;
    char buffer[5000];
    ssize_t bytes_received;
    std::string response;
    std::string full_response;
    while (true)
    {

        if (encryption == true)
        {
            bytes_received = SSL_read(ssl, buffer, sizeof(buffer));
        }
        else
        {
            bytes_received = recv(_socket, buffer, sizeof(buffer), 0);
        }

        response = std::string(buffer, bytes_received);
        std::string tag_str = std::to_string(tag);
        full_response += response;

        if (response.rfind(tag_str + " OK") != std::string::npos ||
            response.rfind("* OK") != std::string::npos ||
            response.rfind(tag_str + " NO") != std::string::npos ||
            response.rfind(tag_str + " BAD") != std::string::npos)
        {
            break;
        }
        if (bytes_received == -1)
        {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                perror("recv");
            }
            // Return empty string to indicate no data available
            return "";
        }
    }
    // std::cout << "Received " << bytes_received << " bytes from " << ip_address << ":" << port << std::endl;
    return full_response;
}