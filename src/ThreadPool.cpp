#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int size) : stop(false) {
  for (unsigned int i = 0; i < size; i++) {
    workers_.emplace_back(std::thread([this]() {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(queue_mutex_);
          // 进入临界区
          condition_variable_.wait(
              lock,
              [this]() { // wait后阻塞，等待lock被释放，并等待条件：“任务队列不为空或线程池停止”，才获得lock，继续执行
                return stop || !tasks_.empty();
              });
          if (stop && tasks_.empty())
            return;
          task = tasks_.front();
          tasks_.pop();
          // 出作用域即lock析构，释放锁，离开临界区
        }
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop = true;
  }
  condition_variable_.notify_all();
  for (std::thread &th : workers_) {
    if (th.joinable()) {
      th.join();
    }
  }
}
