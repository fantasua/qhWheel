#pragma once
#include "message.h"

enum TaskType {
  kRead = 0,
  kWrite = 1,
  kProcess = 2,
  kTotalTaskType,
};

struct AsyncMsg {
  TaskType task_type_; // 带来的事件类型
  int worker_thread; // 分配的工作线程
  InternalMsg msg; // 具体的内部通信消息
};
