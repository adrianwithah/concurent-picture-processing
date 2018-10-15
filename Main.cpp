#include <iostream>
#include <cstring>
#include <assert.h>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"
#include "PicThread.hpp"
#include "Command.hpp"

using namespace std;

const string EMPTY_STRING = "";
const string MISC_THREAD_NAME = "MISCELLANEOUS_THREAD";

void join_threads_and_exit(map<string, PicThread*> *filename_to_threads, CommandSyncer *syncer, PicLibrary *picLib) {
  map<string, PicThread*>::iterator it;
  for (it = filename_to_threads->begin(); it != filename_to_threads->end(); it++) {
    it->second->join();
    delete it->second;
  }

  picLib->free();
  delete picLib;
  delete filename_to_threads;
  delete syncer;

  exit(EXIT_SUCCESS);
}


int main(int argc, char ** argv)
{
  PicLibrary *picLib = new PicLibrary();
  CommandSyncer *syncer = new CommandSyncer();

  map<string, PicThread*> *filename_to_threads = new map<string, PicThread*>();
  PicThread *misc_thread = new PicThread();
  filename_to_threads->insert(pair<string, PicThread*>(MISC_THREAD_NAME, misc_thread));
  misc_thread->run();

  int num_preloads = argc - 1;
  if (num_preloads > 0) {
    for (int i = 0; i < num_preloads; i++) {
      string path = string(argv[1 + i]);
      picLib->loadpicture(path, path.substr(path.find_last_of("/") + 1));
    }
  }

  // write main IO loop that runs the command-line interpreter interactive shell
  while (true) {

    cout << "Command> ";

    string command;
    getline (cin, command);

    cout << "Command entered: " << command << endl;

    vector<string> *tokens = new vector<string>;
    stringstream stream(command);
    string intermediate;
    while (getline(stream, intermediate, ' ')) {
      tokens->push_back(intermediate);
    }

    Command* cmd = new Command(tokens, picLib, filename_to_threads);

    string inst = cmd->get_instruction();
    if (inst == "liststore" || inst == "load" || inst == "unload" || inst == "save" || inst == "display") {
      syncer->add(cmd);
      cmd->reg_syncer(syncer);
    }

    //check if exit and do thread joining here.
    string filename = cmd->get_filename();
    if (filename != EMPTY_STRING) {
      map<string, PicThread*>::iterator it = filename_to_threads->find(filename);
      if (it == filename_to_threads->end()) {
        PicThread *trd = new PicThread();
        it = filename_to_threads->insert(pair<string, PicThread*>(filename, trd)).first;
        trd->run();
      }

      it->second->add(cmd);
      continue;
    }

    //liststore or exit here.
    if (cmd->get_instruction() == "exit") {
      join_threads_and_exit(filename_to_threads, syncer, picLib);
    }

    misc_thread->add(cmd);

    if (cin.eof()) {
      cout << "EOF detected. Waiting for all commands to complete." << endl;

      join_threads_and_exit(filename_to_threads, syncer, picLib);
    }
  }

  delete picLib;

  return 0;
}
