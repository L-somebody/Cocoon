/*
 * Channel is used to distinguish different server.
 * Channel is a encapsulation of methods of epoll serving. A channel must bind with a epoll, and 
 * deal with a fd. Previously epoll could only return fd, now it returns channel for easy management..
 * 
*/

#pragma once
#include "Macros.h"

#include <functional>

class Socket;
class EventLoop;
class Channel {
public:
  Channel(EventLoop *_loop, int _fd);
  ~Channel();

  DISALLOW_COPY_AND_MOVE(Channel);

  void handleEvent();
  void enableRead();

  int getFd();
  uint32_t getListenEvents();
  uint32_t getReadyEvents();
  bool getInEpoll();
  void setInEpoll(bool _in = true);

  void useET();

  void setReadyEvents(uint32_t ev);
  void setReadCallback(std::function<void()> const &_callback);

private:
  EventLoop *loop_;
  int fd_;
  uint32_t listen_events_;
  uint32_t ready_events_;
  bool in_epoll_;
  std::function<void()> read_callback_;
  std::function<void()> write_callback_;
};
