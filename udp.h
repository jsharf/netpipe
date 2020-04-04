#ifndef UDP_H
#define UDP_H

#include <ostream>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <cstring>

class UDPListener
{
    public:
        UDPListener(std::string port, int debug=false);
        int Listen(uint8_t *data, size_t len) const;

       private:
        struct sockaddr_in remaddr_;
        int in_sockfd_;
        bool debug_;
};

class UDPSender
{
    public:
        UDPSender(std::string url, int port, int debug=false);
        int Send(const uint8_t *data, size_t len) const;

       private:
        sockaddr_in out_addr_;
        int out_sockfd_;
        bool debug_;
};

#endif  // UDP_H
