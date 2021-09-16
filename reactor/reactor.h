#pragma once

#include <pthread.h>

#include <list>

#include "task.h"

using std::list;

class Reactor;

namespace Component {

// 内部event事件
struct Event {

};

// Reactor执行的task
struct Task{};

// reactor定义
class Reactor {
 public:
  Reactor();
  ~Reactor();

 public:
  void Init();
  void Start();
  void Stop();
  pthread_t GetThreadId() { return thread_id_; }
  pthread_t SetThreadId(pthread_t id) { thread_id_ = id; }
  std::size_t GetTaskNum() { return task_list_.size(); }

 public:
  bool AppendTask();
  bool DeleteTask();

 private:
 private:
  pthread_t thread_id_;
  list<Event> task_list_;
  int epoll_fd_;
};
};

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