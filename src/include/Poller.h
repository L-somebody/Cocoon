#pragma once
#include "common.h"
#include <vector>

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#ifdef OS_MACOS
#include <sys/event.h>
#endif

class Poller {
public:
    DISALLOW_COPY_AND_MOVE(Poller);
    Poller();
    ~Poller();

    RC UpdateChannel(Channel *ch) const;
    RC DeleteChannel(Channel *ch) const;

    [[nodiscard]] std::vector<Channel *> Poll(int timeout = -1) const;

private:
    int fd_;

#ifdef OS_LINUX
    struct epoll_event *events_{nullptr};
#endif

#ifdef OS_MACOS
    struct kevent *events_;
#endif
};