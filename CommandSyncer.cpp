#include "CommandSyncer.hpp"

CommandSyncer::CommandSyncer() {
  if (pthread_mutex_init(queue_lock, NULL) != 0) {
    cout << "error initialising pthread mutex!" << endl;
  }

  if (pthread_cond_init(cond, NULL) != 0) {
    cout << "error initialising cond!" << endl;
  }
}

void CommandSyncer::sleep_till_turn(Command* cmd) {
  pthread_mutex_lock(cmd->syncer->queue_lock);
  while (sync_queue->front() != cmd) {
    pthread_cond_wait(cmd->syncer->cond, cmd->syncer->queue_lock);
  }
  pthread_mutex_unlock(cmd->syncer->queue_lock);
}

void CommandSyncer::add(Command* cmd) {
  pthread_mutex_lock(queue_lock);
  sync_queue->push_back(cmd);
  pthread_mutex_unlock(queue_lock);
}

void CommandSyncer::pop() {
  pthread_mutex_lock(queue_lock);
  sync_queue->pop_front();
  pthread_cond_broadcast(cond);
  pthread_mutex_unlock(queue_lock);
}

void CommandSyncer::free() {
  delete queue_lock;
  delete sync_queue;
}
