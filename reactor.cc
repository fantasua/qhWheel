#include "reactor.h"

#include <pthread.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <iostream>
#include <mutex>

void Reactor::Init() {
  event_fd_ = eventfd(0, EFD_NONBLOCK);
  epoll_fd_ = epoll_create(1024);
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = event_fd_;
  // 可能是这里epoll事件添加的时候有问题
  std::cout << "epollfd" << event_fd_ << "res"
            << epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event_fd_, &ev) << std::endl;
}

void Reactor::AppendTask(AsyncMsg &&msg) {
  msg_queue_.push_back(msg);
  uint64_t q = 1;
  std::cout << "reactor id " << reactor_id_ << std::endl;
  std::cout << "msg append reactor[" << reactor_id_ << "]" << std::endl;
  std::cout << "msg queue size: " << msg_queue_.size() << std::endl;
  write(event_fd_, &q, sizeof(q));
}

std::size_t Reactor::QueueSize() { return msg_queue_.size(); }

void Reactor::Run(void *param) { run(); }

void Reactor::run() {
  std::cout << "reactor[" << reactor_id_ << "]"
            << "starts" << std::endl;
  uint64_t tmp;
  struct epoll_event listend_ev[1024];
  while (1) {
    int num = epoll_wait(epoll_fd_, listend_ev, 1024, -1);
    std::cout << "thread " << reactor_id_ << " queue size " << msg_queue_.size()
              << std::endl;
    for (int i = 0; i < num; ++i) {
      if (listend_ev[i].data.fd == event_fd_) {
        while (!msg_queue_.empty()) {
          auto it = msg_queue_.begin();
          std::cout << "woker thread: ";
          std::cout << it->worker_thread << std::endl;
          std::cout << "msg counter: ";
          std::cout << it->msg.counter << std::endl;

          msg_queue_.pop_front();
        }

        read(listend_ev[i].data.fd, (void *)&tmp, sizeof(tmp));
        std::cout << tmp << std::endl;
      }
      std::cout << "NUM:" << num << std::endl;
    }
  }
  close(event_fd_);
}

void *Reactor::ThreadWrapper(void *param) {
  ParamWrapper params = *(static_cast<ParamWrapper *>(param));
  params.reactor->Run(nullptr);
}
