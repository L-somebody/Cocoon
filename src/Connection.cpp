#include "Connection.h"

#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#include <utility>

#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#include "util.h"

Connection::Connection(EventLoop *_loop, Socket *_sock)
    : loop_(_loop), sock_(_sock) {
  // use a channel to listen the loop
  if(loop_ != nullptr) {
    channel_ = new Channel(loop_, sock_->GetFd());
    channel_->enableRead();
    channel_->useET();
  }
  // init read and send buffer
  read_buffer_ = new Buffer();
  send_buffer_ = new Buffer();
  // set state
  State::Connected;
}

Connection::~Connection() {
  if(loop_ != nullptr) {
    delete channel_;
  }
  delete sock_;
  delete read_buffer_;
  delete send_buffer_;
}

void Connection::read() {
  if(state_ != State::Connected) { assert("connection state is disconnected!"); }
  read_buffer_->clear();
  if(sock_->IsNonBlocking()) {
    readNonBlocking();
  } else {
    readBlocking();
  }
}

void Connection::write() {
  if(state_ != State::Connected) { assert("connection state is disconnected!"); }
  if(sock_->IsNonBlocking()) {
    writeNonBlocking();
  } else {
    writeBlocking();
  }
  send_buffer_->clear();
}

void Connection::readNonBlocking() {
  int sockfd = sock_->GetFd();
  char buf[1024];  // 这个buf大小无所谓
  while (true) {   // 使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    memset(buf, 0, sizeof(buf));
    ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      read_buffer_->append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) {  // 程序正常中断、继续读取
      printf("continue reading\n");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
      break;
    } else if (bytes_read == 0) {  // EOF，客户端断开连接
      printf("read EOF, client fd %d disconnected\n", sockfd);
      state_ = State::Closed;
      break;
    } else {
      printf("Other error on client fd %d\n", sockfd);
      state_ = State::Closed;
      break;
    }
  }
}

void Connection::writeNonBlocking() {
  int sockfd = sock_->GetFd();
  char buf[send_buffer_->size()];
  memcpy(buf, send_buffer_->toStr(), send_buffer_->size());
  int data_size = send_buffer_->size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = ::write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EINTR) {
      printf("continue writing\n");
      continue;
    }
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    if (bytes_write == -1) {
      printf("Other error on client fd %d\n", sockfd);
      state_ = State::Closed;
      break;
    }
    data_left -= bytes_write;
  }
}

void Connection::readBlocking() {
  int sockfd = sock_->GetFd();
  unsigned int rcv_size = 0;
  socklen_t len = sizeof(rcv_size);
  getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
  char buf[rcv_size];
  ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
  if (bytes_read > 0) {
    read_buffer_->append(buf, bytes_read);
  } else if (bytes_read == 0) {
    printf("read EOF, blocking client fd %d disconnected\n", sockfd);
    state_ = State::Closed;
  } else if (bytes_read == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
}

void Connection::writeBlocking() {
  // 没有处理send_buffer_数据大于TCP写缓冲区，的情况，可能会有bug
  int sockfd = sock_->GetFd();
  ssize_t bytes_write = ::write(sockfd, send_buffer_->toStr(), send_buffer_->size());
  if (bytes_write == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
}

void Connection::close() { delete_connection_callback_(sock_); }

Connection::State Connection::getState() { return state_; }
void Connection::setSendBuffer(const char *str) { send_buffer_->setBuf(str); }
Buffer *Connection::getReadBuffer() { return read_buffer_; }
const char *Connection::readBuffer() { return read_buffer_->toStr(); }
Buffer *Connection::getSendBuffer() { return send_buffer_; }
const char *Connection::sendBuffer() { return send_buffer_->toStr(); }

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> const &callback) {
  delete_connection_callback_ = callback;
}
void Connection::setOnConnectCallback(std::function<void(Connection *)> const &callback) {
  on_connect_callback_ = callback;
  channel_->setReadCallback([this]() { on_connect_callback_(this); });
}

void Connection::getlineSendBuffer() { send_buffer_->getLine(); }

Socket *Connection::getSocket() { return sock_; }