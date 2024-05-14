#pragma once
#include "Macros.h"

#include <functional>

class Socket;
class EventLoop;
class Channel;
class Buffer;
class Connection {
public:
  enum State {
    Invalid = 1,
    Handshaking,
    Connected,
    Closed,
    Failed,
  };
  Connection(EventLoop *_loop, Socket *_sock);
  ~Connection();
  DISALLOW_COPY_AND_MOVE(Connection);

  void read();
  void write();

  void setDeleteConnectionCallback(std::function<void(Socket *)> const &callback);
  void setOnConnectCallback(std::function<void(Connection *)> const &callback);
  State getState();
  void close();
  void setSendBuffer(const char *str);
  Buffer *getReadBuffer();
  const char *readBuffer();
  Buffer *getSendBuffer();
  const char *sendBuffer();
  void getlineSendBuffer();
  Socket *getSocket();

  void onConnect(std::function<void()> fn);

private:
  EventLoop *loop_;
  Socket *sock_;
  Channel *channel_{nullptr};
  State state_{State::Invalid};
  Buffer *read_buffer_{nullptr};
  Buffer *send_buffer_{nullptr};
  std::function<void(Socket *)> delete_connection_callback_;

  std::function<void(Connection *)> on_connect_callback_;

  void readNonBlocking();
  void writeNonBlocking();
  void readBlocking();
  void writeBlocking();
};
