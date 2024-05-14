#include "util.h"
#include <cstdio>
#include <cstdlib>

void errif(bool condition, const char *errormsg) {
  if (condition) {
    perror(errormsg);
    exit(EXIT_FAILURE);
  }
}