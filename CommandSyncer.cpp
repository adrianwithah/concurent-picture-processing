#include "CommandSyncer.hpp"

CommandSyncer::CommandSyncer(){}

bool CommandSyncer::is_my_turn(Command* cmd) {
  queue_lock->lock();
  bool result = (sync_queue->front() == cmd);
  queue_lock->unlock();
  return result;
}

void CommandSyncer::add(Command* cmd) {
  queue_lock->lock();
  sync_queue->push_back(cmd);
  queue_lock->unlock();
}

void CommandSyncer::pop() {
  queue_lock->lock();
  sync_queue->erase(sync_queue->begin());
  queue_lock->unlock();
}
