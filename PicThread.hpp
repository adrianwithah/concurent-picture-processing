#ifndef PICTHREAD_H
#define PICTHREAD_H

#include <mutex>
#include "Command.hpp"

using namespace std;

struct thread_args {
  vector<Command*> *cmd_queue;
  bool *should_run;
  mutex *queue_mutex;
};

class PicThread {
  private:
    vector<Command*> *cmd_queue = new vector<Command*>;
    pthread_t running_thread;
    bool should_run = true;
    mutex *queue_mutex = new mutex();

  public:
    PicThread();
    void add(Command* cmd);
    void run();
    void join();
};

#endif
