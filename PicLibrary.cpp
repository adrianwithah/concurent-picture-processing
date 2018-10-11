#include "PicLibrary.hpp"
#include "Colour.hpp"

using namespace cv;

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

void PicLibrary::savepicture(string filename, string path) {
  map<string, Picture>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Utils utils;
  if (!utils.saveimage(it->second.getimage(), path)) {
    cout << "Failed to save picture." << endl;
  }
}

void PicLibrary::display(string filename) {
  map<string, Picture>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Utils utils;
  utils.displayimage(it->second.getimage());
}

void PicLibrary::invert(string filename) {
  map<string, Picture>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = it->second;
  int width = pic.getwidth();
  int height = pic.getheight();

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      Colour old_pixel = pic.getpixel(x, y);
      Colour new_pixel = Colour(255 - old_pixel.getred(), 255 - old_pixel.getgreen(), 255 - old_pixel.getblue());
      pic.setpixel(x, y, new_pixel);
    }
  }

  cout << "Inverted " << filename << " successfully." << endl;
}

void PicLibrary::grayscale(string filename) {
  map<string, Picture>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = it->second;
  int origWidth = pic.getwidth();
  int origHeight = pic.getheight();

  for (int x = 0; x < origWidth; x++) {
    for (int y = 0; y < origHeight; y++) {
      Colour old_pixel = pic.getpixel(x, y);
      double avg = (old_pixel.getblue() + old_pixel.getgreen() + old_pixel.getred()) / 3;
      Colour new_pixel = Colour(avg, avg, avg);
      pic.setpixel(x, y, new_pixel);
    }
  }

  cout << "Grayscaled " << filename << " successfully." << endl;
}

void PicLibrary::rotate(int angle, string filename) {
  map<string, Picture>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = it->second;
  int origWidth = pic.getwidth();
  int origHeight = pic.getheight();
  Utils utils;

  switch (angle) {
    case 90: {
      Picture new_pic(origHeight, origWidth);
      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic.setpixel(origHeight - y - 1, x, pic.getpixel(x, y));
        }
      }

      it->second = new_pic;
      cout << "Rotated " << filename << " by 90deg clockwise successfully." << endl;
      break;
    }

    case 180: {
      Picture new_pic(origWidth, origHeight);
      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic.setpixel(origWidth - x - 1, origHeight - y - 1, pic.getpixel(x, y));
        }
      }

      it->second = new_pic;
      cout << "Rotated " << filename << " by 180deg clockwise successfully." << endl;
      break;
    }

    case 270: {
      Picture new_pic(origHeight, origWidth);
      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic.setpixel(y, origWidth - x - 1, pic.getpixel(x, y));
        }
      }

      it->second = new_pic;
      cout << "Rotated " << filename << " by 270deg clockwise successfully." << endl;
      break;
    }
  }
}

void PicLibrary::flipVH(char plane, string filename) {

  map<string, Picture>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = it->second;
  int origWidth = pic.getwidth();
  int origHeight = pic.getheight();

  Picture new_pic(origWidth, origHeight);

  switch (plane) {
    case 'H':

      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic.setpixel(origWidth - x - 1, y, pic.getpixel(x, y));
        }
      }

      it->second = new_pic;
      cout << "Rotated " << filename << " horizontally successfully." << endl;

      break;
    case 'V':

      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic.setpixel(x, origHeight - y - 1, pic.getpixel(x, y));
        }
      }

      it->second = new_pic;
      cout << "Flipped " << filename << " vertically successfully." << endl;
      break;
  }
}

void PicLibrary::blur(string filename) {
  map<string, Picture>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = it->second;
  int origWidth = pic.getwidth();
  int origHeight = pic.getheight();

  Picture new_pic(origWidth, origHeight);

  for (int x = 1; x < origWidth - 1; x++) {
    for (int y = 1; y < origHeight - 1; y++) {
      int sumR = 0;
      int sumG = 0;
      int sumB = 0;
      for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
          Colour pixel = pic.getpixel(x + i, y + j);
          sumR += pixel.getred();
          sumG += pixel.getgreen();
          sumB += pixel.getblue();
        }
      }
      new_pic.setpixel(x, y, Colour(sumR / 9, sumG / 9, sumB / 9));
    }

    for (int x = 0; x < origWidth; x++) {
      new_pic.setpixel(x, 0, pic.getpixel(x, 0));
      new_pic.setpixel(x, origHeight - 1, pic.getpixel(x, origHeight - 1));
    }

    for (int y = 1; y < origHeight - 1; y++) {
      new_pic.setpixel(0, y, pic.getpixel(0, y));
      new_pic.setpixel(origWidth - 1, y, pic.getpixel(origWidth - 1, y));
    }
  }

  it->second = new_pic;
  cout << "Blured " << filename << " successfully." << endl;
}
