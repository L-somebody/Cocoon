#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"
#include <vector>

EventLoop::EventLoop() { epoll_ = new Epoll(); }

EventLoop::~EventLoop() { delete epoll_; }

void EventLoop::loop() {
  while (!quit_) {
    std::vector<Channel *> chs;
    chs = epoll_->poll();
    for (auto &ch : chs) {
      ch->handleEvent();
    }
  }
}

void EventLoop::updateChannel(Channel *ch) { epoll_->updateChannel(ch); }
