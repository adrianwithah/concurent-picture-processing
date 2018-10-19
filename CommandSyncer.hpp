#ifndef COMMANDSYNCER_H
#define COMMANDSYNCER_H

#include <mutex>
#include "Command.hpp"

using namespace std;

class Command;

class CommandSyncer {
  private:
    list<Command*> *sync_queue = new list<Command*>;

  public:
    pthread_cond_t *cond = new pthread_cond_t;
    pthread_mutex_t *queue_lock = new pthread_mutex_t;
    CommandSyncer();
    void sleep_till_turn(Command* cmd);
    void add(Command* cmd);
    void pop();
    void free();
};


#endif
