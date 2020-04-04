#include "udp.h"
using namespace std;
const int PACKETSIZE = 2048;

UDPListener::UDPListener(string port, int debug) : debug_(debug) {
  sockaddr_in in_addr;
  in_sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (in_sockfd_ < 0) {
    if (debug_) cerr << "ERROR opening socket" << endl;
    return;
  }
  // set nonblocking mode
  // int flags = fcntl(in_sockfd_, F_GETFL);
  // flags |= O_NONBLOCK;
  // fcntl(in_sockfd_, F_SETFL, flags);

  bzero((char *)&in_addr, sizeof(in_addr));
  int portno = atoi(port.c_str());
  in_addr.sin_family = AF_INET;
  in_addr.sin_addr.s_addr = INADDR_ANY;
  in_addr.sin_port = htons(portno);

  if (::bind(in_sockfd_, (struct sockaddr *)&in_addr, sizeof(in_addr)) < 0) {
    if (debug_) cerr << "ERROR on binding" << endl;
    return;
  }
  if (debug_) cerr << "Listening on port " << portno << endl;
}

int UDPListener::Listen(uint8_t *data, size_t len) const {
  socklen_t addrlen = sizeof(remaddr_);
  int recvlen;
  if (recvlen = recvfrom(in_sockfd_, data, len, 0,
                         (struct sockaddr *)&remaddr_, &addrlen); recvlen < 0) {
    if (debug_) std::cerr << "Error on recvfrom()." << std::endl;
    return -1;
  }
  return recvlen;
}

UDPSender::UDPSender(string url, int port, int debug) : debug_(debug) {
  out_sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (out_sockfd_ < 0) {
    if (debug_) cerr << "ERROR opening socket" << endl;
    return;
  }

  // int flags = fcntl(out_sockfd_, F_GETFL);
  // flags |= O_NONBLOCK;
  // fcntl(out_sockfd_, F_GETFL);

  bzero((char *)&out_addr_, sizeof(out_addr_));
  out_addr_.sin_family = AF_INET;
  out_addr_.sin_port = htons(port);

  struct hostent *hp = gethostbyname(url.c_str());
  if (!hp) {
    if (debug_) cerr << "could not obtain address of " << url << endl;
    return;
  }

  /* put the host's address into the server address structure */
  memcpy((void *)&out_addr_.sin_addr, hp->h_addr_list[0], hp->h_length);
}

int UDPSender::Send(const uint8_t* data, size_t len) const {
  if (int len = sendto(out_sockfd_, data, len, 0, (struct sockaddr *)&out_addr_,
                       sizeof(out_addr_));
      len < 0) {
    if (debug_) std::cerr << "sendto() failed" << std::endl;
    return -1;
  }
  return len;
}
