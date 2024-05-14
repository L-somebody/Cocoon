#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false) {
  for (int i = 0; i < size; i++) {
    threads.emplace_back(std::thread([this]() {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(tasks_mtx);
          // 进入临界区
          cv.wait(
              lock,
              [this]() { // wait后阻塞，等待lock被释放，并等待条件：“任务队列不为空或线程池停止”，才获得lock，继续执行
                return stop || !tasks.empty();
              });
          if (stop && tasks.empty())
            return;
          task = tasks.front();
          tasks.pop();
          // 出作用域即lock析构，释放锁，离开临界区
        }
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(tasks_mtx);
    stop = true;
  }
  cv.notify_all();
  for (std::thread &th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }
}
