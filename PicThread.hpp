#ifndef PICTHREAD_H
#define PICTHREAD_H

#include <mutex>
#include "Command.hpp"

using namespace std;

//fwd declaration.
class Command;

struct thread_args {
  list<Command*> *cmd_queue;
  bool *should_run;
  mutex *queue_mutex;
};

class PicThread {
  private:
    list<Command*> *cmd_queue = new list<Command*>;
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
