#include <cstring>
#include <unistd.h>

#include "Epoll.h"
#include "util.h"
#include "Channel.h"

#define MAX_EVENTS 1000

Epoll::Epoll() {
  epfd_ = epoll_create1(0);
  errif(epfd_ == -1, "epoll create error");
  // hold a MAX_EVENTS size triggered Channel
  events_ = new epoll_event[MAX_EVENTS];
  memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Epoll::~Epoll() {
  if (epfd_ != -1) {
    close(epfd_);
    epfd_ = -1;
  }
  delete[] events_;
}

std::vector<Channel *> Epoll::poll(int timeout) {
  std::vector<Channel *> active_channels;
  int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
  errif(nfds == -1, "epoll wait error");
  for (int i = 0; i < nfds; ++i) {
    Channel *ch = (Channel *)events_[i].data.ptr;
    ch->setReadyEvents(events_[i].events);
    active_channels.push_back(ch);
  }
  return active_channels;
}

void Epoll::updateChannel(Channel *channel) {
  int fd = channel->getFd();
  struct epoll_event ev {};
  ev.data.ptr = channel;
  ev.events = channel->getListenEvents();
  if (!channel->getInEpoll()) {
    errif(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
    channel->setInEpoll();
  } else {
    errif(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
  }
}

void Epoll::deleteChannel(Channel *channel) {
  int fd = channel->getFd();
  errif(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL) == -1, "epoll delete error");
  channel->setInEpoll(false);
}