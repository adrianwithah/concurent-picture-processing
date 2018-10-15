#ifndef COMMAND_H
#define COMMAND_H

#include "PicLibrary.hpp"
#include "PicThread.hpp"
#include "CommandSyncer.hpp"

using namespace std;

class CommandSyncer;
class PicThread;

enum Instruction {
  LISTSTORE,
  LOAD,
  UNLOAD,
  SAVE,
  EXIT,
  DISPLAY,
  INVERT,
  GRAYSCALE,
  ROTATE,
  FLIP,
  BLUR,
  UNRECOGNISED
};

class Command {

  private:
    vector<string> *args;
    PicLibrary *picLib;
    map<string, PicThread*> *filename_to_threads;
    Instruction instr;
    CommandSyncer *syncer = NULL;

  public:
    Command(vector<string> *args, PicLibrary *picLib);
    void parse_arguments();
    string get_filename();
    string get_instruction();
    void execute();
    void reg_syncer(CommandSyncer *syncer);
};

#endif
