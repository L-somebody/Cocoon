#pragma once

#include <arpa/inet.h>
#include <cstring>

class InetAddress {
public:
  struct sockaddr_in serv_addr;
  socklen_t addr_len;

  InetAddress();
  InetAddress(const char *ip, int port);
  ~InetAddress();
};
