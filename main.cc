#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>

#include "reactor.h"

#define MAX_EVENTS 1024

int main() {
  std::cout << "my reactor started" << std::endl;

  int reactor_num = 4;
  Reactor *reactors = new Reactor[reactor_num];
  for (int i = 0; i < reactor_num; ++i) {
    reactors[i].SetReactorId(i);
    reactors[i].Init();
    pthread_t pid;
    ParamWrapper pr;
    pr.reactor = reactors + i;
    pthread_create(&pid, NULL, &Reactor::ThreadWrapper, (void *)&pr);
    reactors[i].SetThreadId(pid);
    sleep(1);
  }

  std::cout << "Running" << std::endl;
  int counter = 0;
  while (1) {
    std::cin >> counter;
    AsyncMsg msg;

    msg.worker_thread = counter % reactor_num;
    std::cout << msg.worker_thread << std::endl;
    msg.msg.counter = counter;
    reactors[msg.worker_thread].AppendTask(std::move(msg));
  }

  std::cout << "End" << std::endl;
  for (int i = 0; i < reactor_num; ++i) {
    pthread_join(reactors[i].GetThreadId(), nullptr);
  }

  return 0;
}