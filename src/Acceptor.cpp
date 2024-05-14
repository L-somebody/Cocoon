#include <utility>

#include "Acceptor.h"
#include "Channel.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop *_loop) : loop_(_loop), sock_(nullptr), channel_(nullptr) {
  sock_ = new Socket();
  InetAddress *addr = new InetAddress("127.0.0.1", 1234);
  sock_->Bind(addr);
  sock_->Listen();
  // sock->setnonblocking();
  channel_ = new Channel(loop_, sock_->GetFd());
  std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
  channel_->setReadCallback(cb);
  channel_->enableRead();
  delete addr;
}

Acceptor::~Acceptor() {
  delete sock_;
  delete channel_;
}

void Acceptor::acceptConnection() {
  InetAddress *clnt_addr = new InetAddress();
  Socket *clnt_sock = new Socket(sock_->Accept(clnt_addr));
  printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->GetFd(), clnt_addr->GetIp(), clnt_addr->GetPort());
  clnt_sock->SetNonBlocking();
  new_connection_callback_(clnt_sock);
  delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> const &callback) {
  new_connection_callback_ = callback;
}