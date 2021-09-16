#pragma once

// task基类
// 用于派生出其他子类型基类
class BaseTask {
 public:
  BaseTask();
  ~BaseTask();

 public:
  virtual void Run() = 0;
};