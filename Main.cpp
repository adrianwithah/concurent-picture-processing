#include <iostream>
#include <cstring>
#include <assert.h>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;

// TODO: Implement the picture library command-line interpreter

int main(int argc, char ** argv)
{
  Utils utils;
  PicLibrary picLib;

  int num_preloads = argc - 1;
  if (num_preloads > 0) {
    for (int i = 0; i < num_preloads; i++) {
      string path = string(argv[1 + i]);
      picLib.loadpicture(path, path.substr(path.find_last_of("/") + 1));
    }
  }

  // write main IO loop that runs the command-line interpreter interactive shell
  while (true) {

    cout << "Command> ";

    string command;
    getline (cin, command);

    cout << command << endl;

    vector<string> tokens;
    stringstream stream(command);
    string intermediate;
    while (getline(stream, intermediate, ' ')) {
      tokens.push_back(intermediate);
    }

    if (tokens[0] == "liststore") {

      picLib.print_picturestore();

    } else if (tokens[0] == "load") {

      if (tokens.size() != 3) {
        cout << "Load usage: load <file_path> <file_name>." << endl;
        continue;
      }

      picLib.loadpicture(tokens[1], tokens[2]);

    } else if (tokens[0] == "unload") {

      if (tokens.size() != 2) {
        cout << "Unload usage: unload <file_name>." << endl;
        continue;
      }

      picLib.unloadpicture(tokens[1]);

    }  else if (tokens[0] == "save") {

      if (tokens.size() != 3) {
        cout << "Save usage: save <file_name> <file_path>." << endl;
        continue;
      }

      picLib.savepicture(tokens[1], tokens[2]);

    }  else if (tokens[0] == "exit") {

      // free stuff here.
      return 0;

    }  else if (tokens[0] == "display") {

      if (tokens.size() != 2) {
        cout << "Display usage: display <file_name>." << endl;
        continue;
      }

      picLib.display(tokens[1]);

    }  else if (tokens[0] == "invert") {

      if (tokens.size() != 2) {
        cout << "Invert usage: invert <file_name>." << endl;
        continue;
      }

      picLib.invert(tokens[1]);

    }  else if (tokens[0] == "grayscale") {

      if (tokens.size() != 2) {
        cout << "Grayscale usage: grayscale <file_name>." << endl;
        continue;
      }

      picLib.grayscale(tokens[1]);

    }  else if (tokens[0] == "rotate") {

      int rotateBy = stoi(tokens[1]);
      if (tokens.size() != 3 || rotateBy != 90 && rotateBy != 180 && rotateBy != 270) {
        cout << "Rotate usage: rotate [90|180|270] <file_name>." << endl;
        continue;
      }

      picLib.rotate(rotateBy, tokens[2]);

    }  else if (tokens[0] == "flip") {

      if (tokens.size() != 3 || tokens[1] != "H" && tokens[1] != "V") {
        cout << "Flip usage: flip [H|V] <file_name>." << endl;
        continue;
      }

      picLib.flipVH(tokens[1][0], tokens[2]);

    }  else if (tokens[0] == "blur") {

      if (tokens.size() != 2) {
        cout << "Blur usage: blur <file_name>." << endl;
        continue;
      }

      picLib.blur(tokens[1]);

    }  else {
      cout << "Unrecognised command.. Please try again." << endl;
      continue;
    }

  }

  return 0;

}
