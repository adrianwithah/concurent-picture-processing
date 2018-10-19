#include "Command.hpp"
#include <pthread.h>

Command::Command(vector<string> *args, PicLibrary *picLib) {
  this->args = args;
  this->picLib = picLib;
  parse_arguments();
}

void Command::reg_syncer(CommandSyncer* syncer) {
  this->syncer = syncer;
}

Instruction Command::get_instruction() {
  return instr;
}

void Command::parse_arguments() {
  if (args->size() == 0) {
    instr = Instruction::UNRECOGNISED;
    return;
  }

  if ((*args)[0] == "liststore") {

    instr = Instruction::LISTSTORE;

  } else if ((*args)[0] == "load") {

    instr = Instruction::LOAD;

  } else if ((*args)[0] == "unload") {

    instr = Instruction::UNLOAD;

  }  else if ((*args)[0] == "save") {

    instr = Instruction::SAVE;

  }  else if ((*args)[0] == "exit") {

    instr = Instruction::EXIT;

  }  else if ((*args)[0] == "display") {

    instr = Instruction::DISPLAY;

  }  else if ((*args)[0] == "invert") {

    instr = Instruction::INVERT;

  }  else if ((*args)[0] == "grayscale") {

    instr = Instruction::GRAYSCALE;

  }  else if ((*args)[0] == "rotate") {

    instr = Instruction::ROTATE;

  }  else if ((*args)[0] == "flip") {

    instr = Instruction::FLIP;

  }  else if ((*args)[0] == "blur") {

    instr = Instruction::BLUR;

  }  else {

    instr = Instruction::UNRECOGNISED;

  }
}

string Command::get_filename() {
  switch (instr) {
    case LISTSTORE:
      return "";
    case LOAD:
      return (*args)[2];
    case UNLOAD:
      return (*args)[1];
    case SAVE:
      return (*args)[1];
    case EXIT:
      return "";
    case DISPLAY:
      return (*args)[1];
    case INVERT:
      return (*args)[1];
    case GRAYSCALE:
      return (*args)[1];
    case ROTATE:
      return (*args)[2];;
    case FLIP:
      return (*args)[2];;
    case BLUR:
      return (*args)[1];
    case UNRECOGNISED:
      return "";
  }
}

void Command::execute() {

  // if (syncer != NULL) {
  //   while (!syncer->is_my_turn(this)) {
  //     pthread_yield();
  //   }
  // }

  switch (instr) {
    case LISTSTORE:
      picLib->print_picturestore();
      break;
    case LOAD:
      if ((*args).size() != 3) {
        cout << "Load usage: load <file_path> <file_name>." << endl;
        break;
      }

      picLib->loadpicture((*args)[1], (*args)[2]);
      break;
    case UNLOAD: {
      if ((*args).size() != 2) {
        cout << "Unload usage: unload <file_name>." << endl;
        break;
      }

      string filename = (*args)[1];
      picLib->unloadpicture(filename);
      break;
    }

    case SAVE:
      if ((*args).size() != 3) {
        cout << "Save usage: save <file_name> <file_path>." << endl;
        break;
      }

      picLib->savepicture((*args)[1], (*args)[2]);
      break;
    case EXIT:
      exit(EXIT_SUCCESS);
      break;
    case DISPLAY:
      if ((*args).size() != 2) {
        cout << "Display usage: display <file_name>." << endl;
        break;
      }

      picLib->display((*args)[1]);
      break;
    case INVERT:
      if ((*args).size() != 2) {
        cout << "Invert usage: invert <file_name>." << endl;
        break;
      }

      picLib->invert((*args)[1]);
      break;
    case GRAYSCALE:
      if ((*args).size() != 2) {
        cout << "Grayscale usage: grayscale <file_name>." << endl;
        break;
      }

      picLib->grayscale((*args)[1]);
      break;
    case ROTATE: {
      int rotateBy = stoi((*args)[1]);
      if ((*args).size() != 3 || rotateBy != 90 && rotateBy != 180 && rotateBy != 270) {
        cout << "Rotate usage: rotate [90|180|270] <file_name>." << endl;
        break;
      }

      picLib->rotate(rotateBy, (*args)[2]);
      break;
    }

    case FLIP:
      if ((*args).size() != 3 || (*args)[1] != "H" && (*args)[1] != "V") {
        cout << "Flip usage: flip [H|V] <file_name>." << endl;
        break;
      }

      picLib->flipVH((*args)[1][0], (*args)[2]);
      break;
    case BLUR:
      if ((*args).size() != 2) {
        cout << "Blur usage: blur <file_name>." << endl;
        break;
      }

      picLib->blur((*args)[1]);
      break;
    case UNRECOGNISED:
      cout << "Unrecognised command.. Please try again." << endl;
      break;
  }

  // if (syncer != NULL && syncer->is_my_turn(this)) {
  //   syncer->pop();
  // }

  if (syncer != NULL) {
    syncer->pop();
  }

  delete args;
}
