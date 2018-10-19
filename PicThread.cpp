#include <pthread.h>

#include "PicThread.hpp"

PicThread::PicThread() {
  if (pthread_mutex_init(queue_mutex, NULL) != 0) {
    cout << "error initialising pthread mutex!" << endl;
  }

  if (pthread_cond_init(picthread_cond, NULL) != 0) {
    cout << "error initialising cond!" << endl;
  }
}

static void *thread_func(void *arg) {

  thread_args *args = (thread_args*) arg;
  list<Command*> *cmd_queue = args->cmd_queue;
  pthread_mutex_t *queue_mutex = args->queue_mutex;
  pthread_cond_t *thread_cond = args->thread_cond;

  while (*(args->should_run) || !(cmd_queue->empty())) {
    pthread_mutex_lock(queue_mutex);
    while (cmd_queue->empty()) {
      if (!*(args->should_run)) {
        cout << "Destroying thread." <<  endl;

        delete args;
        pthread_mutex_unlock(queue_mutex);
        return nullptr;
      }

      pthread_cond_wait(thread_cond, queue_mutex);
    }

    if (!cmd_queue->empty()) {
      Command* cmd = cmd_queue->front();

      if (cmd->syncer != nullptr) {
        cmd->syncer->sleep_till_turn(cmd);
      }

      cmd_queue->pop_front();

      pthread_mutex_unlock(queue_mutex);

      cmd->execute();

      delete cmd;
    } else {
      pthread_mutex_unlock(queue_mutex);
    }
  }

  cout << "Destroying thread." <<  endl;

  delete args;
}

void PicThread::add(Command* cmd) {
  pthread_mutex_lock(queue_mutex);
  cmd_queue->push_back(cmd);
  pthread_cond_broadcast(picthread_cond);
  pthread_mutex_unlock(queue_mutex);
}

void PicThread::run() {
  thread_args* args = new thread_args;
  args->cmd_queue = cmd_queue;
  args->should_run = &should_run;
  args->queue_mutex = queue_mutex;
  args->thread_cond = picthread_cond;

  if (!pthread_create(&running_thread, NULL, &thread_func, args) == 0) {
    cout << "Error occured creating PicThread!" << endl;
  }
}

void PicThread::join() {
  should_run = false;
  // we get all threads to check the boolean in case they are sleeping
  // waiting for the next signal.
  pthread_cond_broadcast(picthread_cond);
  pthread_join(running_thread, NULL);
  delete cmd_queue;
  delete queue_mutex;
}
