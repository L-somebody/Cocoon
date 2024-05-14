/*
 * Encapsulation of epoll.
 * The Epoll is used to deal with channel.
*/

#pragma once
#include "Macros.h"

#include <vector>

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

class Channel;
class Epoll {
public:
  Epoll();
  ~Epoll();

  DISALLOW_COPY_AND_MOVE(Epoll);

  void updateChannel(Channel *ch); // include epoll add and mod
  void deleteChannel(Channel *ch);

  std::vector<Channel *> poll(int timeout = -1);
private:
  int epfd_{1};
  struct epoll_event *events_{nullptr}; // The event that is triggered will store there (channel)
};