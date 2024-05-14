#include "InetAddress.h"
#include <string.h>

InetAddress::InetAddress() : addr_len(sizeof(serv_addr)) {
  bzero(&serv_addr, sizeof(serv_addr));
}

InetAddress::InetAddress(const char *ip, int port)
    : addr_len(sizeof(serv_addr)) {
  bzero(&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(ip);
  serv_addr.sin_port = htons(port);
  addr_len = sizeof(serv_addr);
}

InetAddress::~InetAddress() {}
