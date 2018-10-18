#ifndef PICLIB_H
#define PICLIB_H

#include "Picture.hpp"
#include "Utils.hpp"

using namespace std;

/*
Thread-safety strategy:
The general methodology is to create PicThreads. Each PicThread is a thread
responsible for all commands on a specific filename, including returning error
responses after being unloaded. This design decision will be explained further
below. Each PicThread has a cmd_queue, which holds the commands it has yet to
process. Calling PicThread.run() will create a pthread. This pthread will run in
a while loop on a boolean that PicThread can set to cause the pthread to stop
looping and exit. Within the while loop, the pthread will keep checking the
command queue for jobs and execute them. Currently, this is done through
busy-waiting, because I failed to resolve the bugs created through
pthread_cond_wait, which will be a better option.

We have to maintain the order of operations entered for any specific picture.
This is why it makes sense to have one thread responsible for all commands
towards a picture (or specifically the filename), as we are able to easily
synchronize execution through the cmd_queue.

We have to maintain the absolute order of (LISTSTORE, LOAD, UNLOAD, DISPLAY,
SAVE) regardless of any picture it targets. To do this, I created the
CommandSyncer class. This essentially holds a global queue that synchronizes
the order of execution of the above mentioned commands. These commands are added
to the syncer immediately after they are parsed in the constructor new Command();
Then, just any thread executes a command in Command.execute(), it checks if
the command has a syncer. If it does, it means the command has to follow a
specific order. Then, it will check whether the front of the queue is the same
as the current command about to be executed. If so, the command is executed.
Else we busy-wait (for now) for our turn. Only on the command's turn and after
executing the command will we pop the command off the syncer's queue. This is
extensible to every Command holding a list of CommandSyncers, which will allow
us to sync multiple commands in any possible combinations.
*/

class PicLibrary {

  private:
    map<string, Picture*> picStore;

  public:
  // defaiult constructor/deconstructor
  PicLibrary(){};
  ~PicLibrary(){};

  //lock routines
  bool lock();
  void unlock();

  // command-line interpreter routines
  void print_picturestore();
  void loadpicture(string path, string filename);
  void unloadpicture(string filename);
  void savepicture(string filename, string path);
  void display(string filename);

  // picture transformation routines
  void invert(string filename);
  void grayscale(string filename);
  void rotate(int angle, string filename);
  void flipVH(char axis, string filename);
  void blur(string filename);
  void free();
};

#endif
