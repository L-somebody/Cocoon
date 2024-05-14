#pragma once

#include <functional>
#include <map>
#include <vector>

#include "Macros.h"

class Socket;
class EventLoop;
class Acceptor;
class Connection;
class ThreadPool;
class Server {
private:
  EventLoop *main_reactor_;
  Acceptor *acceptor_;
  std::map<int, Connection *> connections_;
  std::vector<EventLoop *> sub_reactors_;
  ThreadPool *thread_pool_;
  std::function<void(Connection *)> on_connect_callback_;

public:
  explicit Server(EventLoop *);
  ~Server();

  DISALLOW_COPY_AND_MOVE(Server);

  void newConnection(Socket *sock);
  void deleteConnection(Socket *sock);
  void onConnect(std::function<void(Connection *)> fn);
};