#include <pthread.h>

#include "PicThread.hpp"

PicThread::PicThread(){}

static void *thread_func(void *arg) {
  thread_args *args = (thread_args*) arg;
  vector<Command*> *cmd_queue = args->cmd_queue;
  mutex *queue_mutex = args->queue_mutex;

  while (*(args->should_run) || !(cmd_queue->empty())) {
    queue_mutex->lock();
    if (!cmd_queue->empty()) {
      // cout << "Found command to execute!" << endl;
      Command* cmd = cmd_queue->front();
      cmd_queue->erase(cmd_queue->begin());

      queue_mutex->unlock();

      cmd->execute();

      delete cmd;
    } else {
      queue_mutex->unlock();
    }
  }

  cout << "Destroying thread." <<  endl;

  delete args;
}

void PicThread::add(Command* cmd) {
  queue_mutex->lock();
  cmd_queue->push_back(cmd);
  cout << "Added command to queue: " << cmd->get_instruction() << endl;
  queue_mutex->unlock();
}

void PicThread::run() {
  thread_args* args = new thread_args;
  args->cmd_queue = cmd_queue;
  args->should_run = &should_run;
  args->queue_mutex = queue_mutex;

  if (pthread_create(&running_thread, NULL, &thread_func, args) == 0) {
    // cout << "Thread created successfully!" << endl;
  }
}

void PicThread::join() {
  should_run = false;
  pthread_join(running_thread, NULL);
}
