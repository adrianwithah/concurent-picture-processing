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
  pthread_mutex_t *queue_mutex;
  pthread_cond_t *thread_cond;
};

class PicThread {
  private:
    list<Command*> *cmd_queue = new list<Command*>;
    pthread_t running_thread;
    bool should_run = true;
    pthread_mutex_t *queue_mutex = new pthread_mutex_t;
    pthread_cond_t *picthread_cond = new pthread_cond_t;

  public:
    PicThread();
    void add(Command* cmd);
    void run();
    void join();
};

#endif
