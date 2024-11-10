#include "client.h"


// creating connection
Client::Client(std::string ip_address, std::string port_, bool encryption_, std::string cert_file_, std::string cert_dir_)
{
    this->ip_address_ = ip_address;
    this->port_ = port_;
    this->encryption_ = encryption_;

    if (encryption_)
    {
        init_openssl(cert_file_, cert_dir_);
    }
    connect();
}

Client::~Client()
{
    close(_socket);
    if (encryption_)
    {
        SSL_free(ssl);
        SSL_CTX_free(ctx);
    }
    EVP_cleanup();
}

void Client::init_openssl(std::string cert_file_, std::string cert_dir_)
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_client_method());

    const char *cert_file__ptr = cert_file_.empty() ? nullptr : cert_file_.c_str();

    if (!SSL_CTX_load_verify_locations(ctx, cert_file__ptr, cert_dir_.c_str()))
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
        std::cerr << "Error: No certificate provided by the server_" << std::endl;
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
    if ((status = getaddrinfo(ip_address_.c_str(), port_.c_str(), &hints, &servinfo)) != 0)
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

    // encryption_

    if (encryption_ == true)
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
    std::cout << "SENDING " << message << std::endl;
    if (encryption_ == true)
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
std::pair<std::string, bool> Client::receive(int tag)
{
    char buffer[5000];
    ssize_t bytes_received;
    std::string response;
    std::string full_response;
    bool bye = false;

    while (true)
    {

        if (encryption_ == true)
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

        //if there was also a BYE message, we need to break
        if(response.rfind(tag_str + " OK BYE") != std::string::npos){
            bye = true;
            break;
        }

        if ((response.rfind(tag_str + " OK") != std::string::npos) || (
            response.rfind("* OK") != std::string::npos))
        {
            break;
        }
        
        else if (response.rfind(tag_str + " NO") != std::string::npos ||
                 response.rfind(tag_str + " BAD") != std::string::npos)
        {
            std::cerr << "ERROR: Problem with server_" << std::endl;
            exit(1);
        }
        // else{
        //     std::cout << "TOTO - " << tag_str + " OK" << std::endl;
        // }

        if (bytes_received == -1)
        {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                perror("recv");
            }
            // Return empty string to indicate no data available
            return std::make_pair("", bye);
        }
    }
    // std::cout << "Received " << bytes_received << " bytes from " << ip_address << ":" << port_ << std::endl;
    return std::make_pair(full_response, bye);
}