/*
 * Channel is used to distinguish different server.
 * Channel is a encapsulation of methods of epoll serving. A channel must bind with a epoll, and 
 * deal with a fd. Previously epoll could only return fd, now it returns channel for easy management..
 * 
*/

#pragma once
#include "common.h"

#include <functional>

class Socket;
class EventLoop;
class Channel {
public:
    DISALLOW_COPY_AND_MOVE(Channel);

    Channel(int fd, EventLoop *_loop);
    ~Channel();

    void HandleEvent() const;
    void EnableRead();
    void EnableWrite();

    int fd() const;
    short listen_events() const;
    short ready_events() const;
    bool exist() const;
    void set_exist(bool _in = true);
    void EnableET();

      void set_ready_events(int ev);
      void set_read_callback(std::function<void()> const &_callback);
      void set_write_callback(std::function<void()> const &_callback);

      static const int READ_EVENT;
      static const int WRITE_EVENT;
      static const int ET;

private:
      EventLoop *loop_; // related to an event loop
      int fd_;
      short listen_events_;
      short ready_events_;
      bool exist_;
      std::function<void()> read_callback_;
      std::function<void()> write_callback_;
};
