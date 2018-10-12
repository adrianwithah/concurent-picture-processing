#include "Command.hpp"

Command::Command(vector<string> *args, PicLibrary *picLib, map<string, PicThread*> *filename_to_threads) {
  this->args = args;
  this->picLib = picLib;
  this->filename_to_threads = filename_to_threads;
  parse_arguments();
}

string Command::get_instruction() {
  switch (instr) {
    case LISTSTORE:
      return "liststore";
    case LOAD:
      return "load";
    case UNLOAD:
      return "unload";
    case SAVE:
      return "save";
    case EXIT:
      return "exit";
    case DISPLAY:
      return "display";
    case INVERT:
      return "invert";
    case GRAYSCALE:
      return "grayscale";
    case ROTATE:
      return "rotate";
    case FLIP:
      return "flip";
    case BLUR:
      return "blur";
    case UNRECOGNISED:
      return "unrecognised";
  }
}

void Command::parse_arguments() {
  if ((*args).size() == 0) {
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

  cout << "Executing command!" << endl;

  switch (instr) {
    case LISTSTORE:
      picLib->print_picturestore();
      break;
    case LOAD:
      if ((*args).size() != 3) {
        cout << "Load usage: load <file_path> <file_name>." << endl;
        return;
      }

      picLib->loadpicture((*args)[1], (*args)[2]);

      // create PicThread and add to list.
      break;
    case UNLOAD: {
      if ((*args).size() != 2) {
        cout << "Unload usage: unload <file_name>." << endl;
        return;
      }

      string filename = (*args)[1];
      picLib->unloadpicture(filename);

      // map<string, PicThread*>::iterator it = filename_to_threads->find(filename);
      // if (it != filename_to_threads->end()) {
      //   it->second->join();
      //   filename_to_threads->erase(filename);
      // }

      break;
    }

    case SAVE:
      if ((*args).size() != 3) {
        cout << "Save usage: save <file_name> <file_path>." << endl;
        return;
      }

      picLib->savepicture((*args)[1], (*args)[2]);
      cout << "Saved " << (*args)[1] << " to " << (*args)[2] << endl;
      break;
    case EXIT:
      // free stuff here.
      exit(EXIT_SUCCESS);

      break;
    case DISPLAY:
      if ((*args).size() != 2) {
        cout << "Display usage: display <file_name>." << endl;
        return;
      }

      picLib->display((*args)[1]);
      break;
    case INVERT:
      if ((*args).size() != 2) {
        cout << "Invert usage: invert <file_name>." << endl;
        return;
      }

      picLib->invert((*args)[1]);
      break;
    case GRAYSCALE:
      if ((*args).size() != 2) {
        cout << "Grayscale usage: grayscale <file_name>." << endl;
        return;
      }

      picLib->grayscale((*args)[1]);
      break;
    case ROTATE: {
      int rotateBy = stoi((*args)[1]);
      if ((*args).size() != 3 || rotateBy != 90 && rotateBy != 180 && rotateBy != 270) {
        cout << "Rotate usage: rotate [90|180|270] <file_name>." << endl;
        return;
      }

      picLib->rotate(rotateBy, (*args)[2]);
      break;
    }

    case FLIP:
      if ((*args).size() != 3 || (*args)[1] != "H" && (*args)[1] != "V") {
        cout << "Flip usage: flip [H|V] <file_name>." << endl;
        return;
      }

      picLib->flipVH((*args)[1][0], (*args)[2]);
      break;
    case BLUR:
      if ((*args).size() != 2) {
        cout << "Blur usage: blur <file_name>." << endl;
        return;
      }

      picLib->blur((*args)[1]);
      break;
    case UNRECOGNISED:
      cout << "Unrecognised command.. Please try again." << endl;
      break;
  }

  // if ((*args).size() == 0) {
  //   cout << "Unrecognised command.. Please try again." << endl;
  //   return;
  // }
  //
  // if ((*args)[0] == "liststore") {
  //
  //   picLib->print_picturestore() ;
  //
  // } else if ((*args)[0] == "load") {
  //
  //   if ((*args).size() != 3) {
  //     cout << "Load usage: load <file_path> <file_name>." << endl;
  //     return;
  //   }
  //
  //   picLib->loadpicture((*args)[1], (*args)[2]);
  //
  //   // create PicThread and add to list.
  //
  // } else if ((*args)[0] == "unload") {
  //
  //   if ((*args).size() != 2) {
  //     cout << "Unload usage: unload <file_name>." << endl;
  //     return;
  //   }
  //
  //   picLib->unloadpicture((*args)[1]);
  //
  // }  else if ((*args)[0] == "save") {
  //
  //   if ((*args).size() != 3) {
  //     cout << "Save usage: save <file_name> <file_path>." << endl;
  //     return;
  //   }
  //
  //   picLib->savepicture((*args)[1], (*args)[2]);
  //
  // }  else if ((*args)[0] == "exit") {
  //
  //   // free stuff here.
  //   return;
  //
  // }  else if ((*args)[0] == "display") {
  //
  //   if ((*args).size() != 2) {
  //     cout << "Display usage: display <file_name>." << endl;
  //     return;
  //   }
  //
  //   picLib->display((*args)[1]);
  //
  // }  else if ((*args)[0] == "inverint pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);t") {
  //
  //   if ((*args).size() != 2) {
  //     cout << "Invert usage: invert <file_name>." << endl;
  //     return;
  //   }
  //
  //   picLib->invert((*args)[1]);
  //
  // }  else if ((*args)[0] == "grayscale") {
  //
  //   if ((*args).size() != 2) {
  //     cout << "Grayscale usage: grayscale <file_name>." << endl;
  //     return;
  //   }
  //
  //   picLib->grayscale((*args)[1]);
  //
  // }  else if ((*args)[0] == "rotate") {
  //
  //   int rotateBy = stoi((*args)[1]);
  //   if ((*args).size() != 3 || rotateBy != 90 && rotateBy != 180 && rotateBy != 270) {
  //     cout << "Rotate usage: rotate [90|180|270] <file_name>." << endl;
  //     return;
  //   }
  //
  //   picLib->rotate(rotateBy, (*args)[2]);
  //
  // }  else if ((*args)[0] == "flip") {
  //
  //   if ((*args).size() != 3 || (*args)[1] != "H" && (*args)[1] != "V") {
  //     cout << "Flip usage: flip [H|V] <file_name>." << endl;
  //     return;
  //   }
  //
  //   picLib->flipVH((*args)[1][0], (*args)[2]);
  //
  // }  else if ((*args)[0] == "blur") {
  //
  //   if ((*args).size() != 2) {
  //     cout << "Blur usage: blur <file_name>." << endl;
  //     return;
  //   }
  //
  //   picLib->blur((*args)[1]);
  //
  // }  else {
  //   cout << "Unrecognised command.. Please try again." << endl;
  //   return;
  // }
}
