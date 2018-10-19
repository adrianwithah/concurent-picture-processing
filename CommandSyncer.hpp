#ifndef COMMANDSYNCER_H
#define COMMANDSYNCER_H

#include <mutex>
#include "Command.hpp"

using namespace std;

class Command;

class CommandSyncer {
  private:
    list<Command*> *sync_queue = new list<Command*>;
    mutex *queue_lock = new mutex;

  public:
    CommandSyncer();
    bool is_my_turn(Command* cmd);
    void add(Command* cmd);
    void pop();
    void free();
};


#endif
