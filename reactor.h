#pragma once

#include <pthread.h>

#include <list>

#include "task.h"

using std::list;

class Reactor;

struct ParamWrapper {
  Reactor *reactor;
};

class Reactor {
 public:
 public:
  static void AddReactor(Reactor *reactor);

 public:
  void Init();
  void SetThreadId(pthread_t id) { thread_id_ = id; }
  void SetReactorId(int id) { reactor_id_ = id; }
  pthread_t GetThreadId() { return thread_id_; }
  void AppendTask(AsyncMsg &&msg);
  static void* ThreadWrapper(void *param);
  void Run(void *param);
  void run();
  std::size_t QueueSize();

 private:
  list<AsyncMsg> msg_queue_;
  int reactor_id_;
  pthread_t thread_id_;
  int event_fd_;  // 用于事件唤醒的fd
  int epoll_fd_;
};