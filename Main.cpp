#include <iostream>
#include <cstring>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;

// TODO: Implement the picture library command-line interpreter

int main(int argc, char ** argv)
{
  cout << "------------------------------------------------------" << endl;
  cout << "TODO: Implement the C++ Picture Processing Interpreter" << endl;
  cout << "------------------------------------------------------" << endl;

  // image files can be passed here. Will need to parse and pre-load them into the
  // PicLibrary class store, using the file's base name as key.

  PicLibrary picLib;

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

    // for(int i = 0; i < tokens.size(); i++)
    //     cout << tokens[i] << '\n';

    if (tokens[0] == "liststore") {

      picLib.print_picturestore();

    } else if (tokens[0] == "load") {

      picLib.loadpicture(tokens[1], tokens[2]);

    } else if (tokens[0] == "unload") {

      picLib.unloadpicture(tokens[1]);

    }  else if (tokens[0] == "save") {

    }  else if (tokens[0] == "exit") {

    }  else if (tokens[0] == "display") {

    }  else if (tokens[0] == "invert") {

    }  else if (tokens[0] == "grayscale") {

    }  else if (tokens[0] == "rotate") {

    }  else if (tokens[0] == "flip") {

    }  else if (tokens[0] == "blur") {

    }  else {
      cout << "Unrecognised command.. Please try again." << endl;
      continue;
    }

  }

  return 0;

}
