#include "tcp.h"

TCPListener::TCPListener(int port, int debug)
    : port_(port), debug_(debug), connected_(false), initialized_(false) {}

bool TCPListener::Initialize(int backlog) {
  in_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (in_sockfd_ < 0) {
    if (debug_) std::cerr << "ERROR opening socket" << std::endl;
    return false;
  }

  sockaddr_in in_addr;
  bzero((char *)&in_addr, sizeof(in_addr));
  in_addr.sin_family = AF_INET;
  in_addr.sin_addr.s_addr = INADDR_ANY;
  in_addr.sin_port = htons(port_);

  if (::bind(in_sockfd_, (struct sockaddr *)&in_addr, sizeof(in_addr)) < 0) {
    if (debug_) std::cerr << "ERROR on binding" << std::endl;
    return false;
  }

  if (listen(in_sockfd_, backlog) < 0) {
    if (debug_) std::cerr << "Error on listen()." << std::endl;
    return false;
  }

  if (debug_) std::cerr << "Listening on port " << port_ << std::endl;
  initialized_ = true;
  return true;
}

bool TCPListener::WaitForConnection() {
  if (!initialized_) {
    if (debug_)
      std::cerr << "WaitForConnection() called on uninitialized TCPListener."
                << std::endl;
    return false;
  }

  socklen_t addrlen = sizeof(struct sockaddr_in);
  if (accept(in_sockfd_, reinterpret_cast<struct sockaddr *>(&peer_address_),
             &addrlen) < 0) {
    if (debug_) std::cerr << "Unable to accept() incoming connection." << std::endl;
    return false;
  }

  connected_ = true;
  return true;
}


int TCPListener::Read(uint8_t *data, size_t len) const {
  if (!connected_) {
    if (debug_)
      std::cerr << "Read() called on unconnected TCPListener." << std::endl;
    return -1;
  }
  return read(in_sockfd_, &data, len);
}

int TCPListener::Write(const uint8_t *data, size_t len) const {
  if (!connected_) {
    if (debug_)
      std::cerr << "Write() called on unconnected TCPListener." << std::endl;
    return -1;
  }
  return write(in_sockfd_, &data, len);
}

TCPSender::TCPSender(std::string url, int port, int debug)
    : url_(url),
      port_(port),
      debug_(debug),
      connected_(false),
      initialized_(false) {}

bool TCPSender::Initialize() {
  out_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (out_sockfd_ < 0) {
    if (debug_) std::cerr << "ERROR opening socket" << std::endl;
    return false;
  }

  // int flags = fcntl(out_sockfd, F_GETFL);
  // flags |= O_NONBLOCK;
  // fcntl(out_sockfd, F_GETFL);
  initialized_ = true;
  return true;
}

bool TCPSender::Connect() {
  sockaddr_in out_addr;
  bzero((char *)&out_addr, sizeof(out_addr));
  out_addr.sin_family = AF_INET;
  out_addr.sin_port = htons(port_);

  struct hostent *hp = gethostbyname(url_.c_str());
  if (!hp) {
    if (debug_) std::cerr << "could not obtain address of " << url_ << std::endl;
    return false;
  }

  /* put the host's address into the server address structure */
  memcpy((void *)&out_addr.sin_addr, hp->h_addr_list[0], hp->h_length);

  if (connect(out_sockfd_, reinterpret_cast<const sockaddr *>(&out_addr), sizeof(struct sockaddr_in)) < 0) {
    if (debug_) std::cerr << "Error calling connect()." << std::endl;
    return false;
  }

  connected_ = true;
  return true;
}

int TCPSender::Read(uint8_t *data, size_t len) const {
  if (!connected_) {
    if (debug_)
      std::cerr << "Read() called on unconnected TCPSender." << std::endl;
    return -1;
  }
  return read(out_sockfd_, data, len);
}

int TCPSender::Write(const uint8_t *data, size_t len) const {
  if (!connected_) {
    if (debug_)
      std::cerr << "Write() called on unconnected TCPSender." << std::endl;
    return -1;
  }
  return write(out_sockfd_, data, len);
}
