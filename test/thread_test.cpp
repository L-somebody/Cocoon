#include <iostream>
#include <string>

#include "ThreadPool.h"

void print(int a, double b, const char *c, std::string const &d) {std::cout << a << b << c << d << std::endl;}

void Test() {std::cout << "hellp" << std::endl;}

int main(int argc, const char *argv[]) {
    ThreadPool *poll = new ThreadPool();
    std::function<void()> func = std::bind(print, 1, 3.14, "hello", std::string("world"));
    poll->add(func);
    func = Test;
    poll->add(func);
    delete poll;
    return 0;
}