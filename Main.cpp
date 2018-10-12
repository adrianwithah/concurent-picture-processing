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

int main(int argc, char ** argv)
{
  PicLibrary *picLib = new PicLibrary();

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
      map<string, PicThread*>::iterator it;
      for (it = filename_to_threads->begin(); it != filename_to_threads->end(); it++) {
        it->second->join();
      }

      exit(EXIT_SUCCESS);
    }

    misc_thread->add(cmd);

    if (cin.eof()) {
      cout << "EOF detected. Waiting for all commands to complete." << endl;

      map<string, PicThread*>::iterator it;
      for (it = filename_to_threads->begin(); it != filename_to_threads->end(); it++) {
        it->second->join();
      }

      exit(EXIT_SUCCESS);
    }
  }

  delete picLib;

  return 0;
}
