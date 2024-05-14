#pragma once
#include <string>
#include "Macros.h"

class Buffer {
private:
  std::string buf;

public:
  Buffer() = default;
  ~Buffer() = default;

  DISALLOW_COPY_AND_MOVE(Buffer);

  void append(const char *_str, int _size);
  ssize_t size();
  const char *toStr();
  void clear();
  void getLine();
  void setBuf(const char *);
};