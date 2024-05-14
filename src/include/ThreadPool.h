#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include "Macros.h"

class ThreadPool {
private:
  std::vector<std::thread> threads;
  std::queue<std::function<void()>> tasks;
  std::mutex tasks_mtx;
  std::condition_variable cv;
  bool stop;

public:
  ThreadPool(int size = std::thread::hardware_concurrency());
  ~ThreadPool();

  template <class F, class... Args>
  auto add(F &&f, Args &&... args)
      -> std::future<typename std::result_of<F(Args...)>::type>;
};

template <class F, class... Args>
auto ThreadPool::add(F &&f, Args &&... args)
    -> std::future<typename std::result_of<
        F(Args...)>::type> { // 尾置返回类型是一个期物，用作异步处理
  using return_type = typename std::result_of<F(Args...)>::
      type; // 送入模板类的线程运行函数返回结果就是该线程的期望得到的结果

  // 这一句太难理解了
  auto task = std::make_shared<std::packaged_task<
      return_type()>>( // task是一个智能指针，std::packaged_task封装一个异步任务
      std::bind(
          std::forward<F>(f),
          std::forward<Args>(args)...) // 用std::bind绑定函数和参数，延迟调用
  );

  std::future<return_type> res =
      task->get_future(); // std::packaged_task封装有期物
  { // 进入临界区，操作所有线程共有的线程池
    std::unique_lock<std::mutex> lock(tasks_mtx); // 锁住

    if (stop) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    tasks.emplace([task]() {
      (*task)();
    }); // 通过lambda表达式，生成调用智能指针（task）指向的可调用对象的函数对象
  }
  cv.notify_one(); // 无条件唤醒一个线程
  return res;
} // 将任务添加之后，会从线程池取得一个线程进行运行，返回期物对象