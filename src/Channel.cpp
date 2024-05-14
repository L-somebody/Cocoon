#include "Channel.h"

#include <sys/epoll.h>
#include <unistd.h>
#include <utility>

#include "EventLoop.h"
#include "Socket.h"

Channel::Channel(EventLoop *_loop, int _fd)
    : loop_(_loop), fd_(_fd), listen_events_(0), ready_events_(0), in_epoll_(false) {}

Channel::~Channel() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void Channel::handleEvent() {
  if (ready_events_ & (EPOLLIN | EPOLLPRI)) {
    read_callback_();
  }
  if (ready_events_ & (EPOLLOUT)) {
    write_callback_();
  }
}

void Channel::enableRead() {
  listen_events_ |= EPOLLIN | EPOLLET;
  loop_->updateChannel(this);
}

void Channel::useET() {
  listen_events_ |= EPOLLET;
  loop_->updateChannel(this);
}

int Channel::getFd() { return fd_; }

uint32_t Channel::getListenEvents() { return listen_events_; }

uint32_t Channel::getReadyEvents() { return ready_events_; }

bool Channel::getInEpoll() { return in_epoll_; }

void Channel::setInEpoll(bool _in) { in_epoll_ = _in; }

void Channel::setReadyEvents(uint32_t _ev) { ready_events_ = _ev; }

void Channel::setReadCallback(std::function<void()> const &_callback) {
  read_callback_ = _callback;
}
