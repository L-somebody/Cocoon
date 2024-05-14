#pragma once
#include "Macros.h"

#include <functional>

class Epoll;
class Channel;
class EventLoop {
private:
  Epoll *epoll_{nullptr};
  bool quit_{false};

public:
  EventLoop();
  ~EventLoop();

  DISALLOW_COPY_AND_MOVE(EventLoop);

  void loop();
  void updateChannel(Channel *ch);

  void addThread(std::function<void()>);
};