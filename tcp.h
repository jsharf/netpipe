#ifndef TCP_H
#define TCP_H

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

extern const int PACKETSIZE;

struct control;

class TCPListener {
 public:
  TCPListener(int port, int debug = false);
  bool Initialize(int backlog=10);
  bool WaitForConnection();
  int Read(uint8_t *data, size_t len) const;
  int Write(const uint8_t *data, size_t len) const;

 private:
  int port_;
  struct sockaddr_in peer_address_;
  int in_sockfd_;
  bool debug_;
  bool connected_;
  bool initialized_;
};

class TCPSender {
 public:
  TCPSender(std::string url, int port, int debug = false);
  bool Initialize();
  bool Connect();
  int Read(uint8_t *data, size_t len) const;
  int Write(const uint8_t *data, size_t len) const;

 private:
  std::string url_;
  int port_;
  struct sockaddr_in peer_address_;
  int out_sockfd_;
  bool debug_;
  bool connected_;
  bool initialized_;
};

#endif  // TCP_H
