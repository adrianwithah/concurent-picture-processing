#ifndef COMMANDSYNCER_H
#define COMMANDSYNCER_H

#include <mutex>
#include "Command.hpp"

using namespace std;

class Command;

class CommandSyncer {
  private:
    vector<Command*> *sync_queue = new vector<Command*>;
    mutex *queue_lock = new mutex;

  public:
    CommandSyncer();
    bool is_my_turn(Command* cmd);
    void add(Command* cmd);
    void pop();
};


#endif
