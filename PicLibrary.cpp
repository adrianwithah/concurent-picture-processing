#include "PicLibrary.hpp"
#include "Colour.hpp"

void PicLibrary::print_picturestore() {
  for (auto it = picStore.cbegin(); it != picStore.cend(); it++) {
    cout << "Image name: " << it->first << endl;
  }
}

void PicLibrary::loadpicture(string path, string filename) {
  Picture pic(path);

  if (pic.getimage().empty()) {
    cout << "Cannot find image at: " << path << endl;
    return;
  }

  if (!picStore.insert(pair<string, Picture>(filename, pic)).second) {
    cout << "Picture with name: " << filename << " already exists!" << endl;
    return;
  }

  cout << "Loaded file: " << filename << endl;
}

void PicLibrary::unloadpicture(string filename) {
  if (picStore.erase(filename) > 0) {
    cout << "Removed picture: " << filename << endl;
  } else {
    cout << "Failed to remove picture: " << filename << endl;
  }
}
