#include <iostream>
#include <cstring>
#include <assert.h>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"
#include "Command.hpp"

using namespace std;

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

    if (cin.eof()) {
      cout << "EOF detected." << endl;
      break;
    }

    vector<string> tokens;
    stringstream stream(command);
    string intermediate;
    while (getline(stream, intermediate, ' ')) {
      tokens.push_back(intermediate);
    }

    Command cmd(&tokens, &picLib);
    cmd.execute();
  }

  return 0;
}
