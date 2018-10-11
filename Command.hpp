#ifndef COMMAND_H
#define COMMAND_H

#include "PicLibrary.hpp"

using namespace std;

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
  Instruction instr;

  public:
  Command(vector<string> *args, PicLibrary *picLib);
  void parse_arguments();
  string get_filename();
  void execute();

};

#endif
