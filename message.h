#pragma once

struct InternalMsg {
  int msg_len_;
  int counter;
  void *p_msg_;
};