#include "PicLibrary.hpp"
#include "Colour.hpp"

#include <pthread.h>
#include <thread>

using namespace cv;

void PicLibrary::print_picturestore() {
  for (auto it = picStore.cbegin(); it != picStore.cend(); it++) {
    cout << "Image name: " << it->first << endl;
  }
}

void PicLibrary::loadpicture(string path, string filename) {
  Picture *pic = new Picture(path);

  if (pic->getimage().empty()) {
    cout << "Cannot find image at: " << path << endl;
    cout << "Load empty delete 1" << endl;
    delete pic;
    cout << "Load empty delete 2" << endl;
    return;
  }

  if (!picStore.insert(pair<string, Picture*>(filename, pic)).second) {
    cout << "Picture with name: " << filename << " already exists!" << endl;
    cout << "insert failed delete 1" << endl;
    delete pic;
    cout << "insert Failed delete 2" << endl;
    return;
  }

  cout << "Loaded file: " << filename << endl;
}

void PicLibrary::unloadpicture(string filename) {
  map<string, Picture*>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist, saving failed." << endl;
    return;
  }

  cout << "Unload delete  1" << endl;
  delete it->second;
  picStore.erase(it);
  cout << "Unload delete 2" << endl;

  cout << "Removed picture: " << filename << endl;
}

void PicLibrary::savepicture(string filename, string path) {
  map<string, Picture*>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist, saving failed." << endl;
    return;
  }

  Utils utils;
  if (!utils.saveimage(it->second->getimage(), path)) {
    cout << "Failed to save picture." << endl;
    return;
  }

  cout << "Saved " << filename << " to " << path << endl;
}

void PicLibrary::display(string filename) {
  map<string, Picture*>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Utils utils;
  utils.displayimage(it->second->getimage());
}

void PicLibrary::invert(string filename) {
  map<string, Picture*>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = *(it->second);
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
  map<string, Picture*>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = *(it->second);
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
  map<string, Picture*>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = *(it->second);
  int origWidth = pic.getwidth();
  int origHeight = pic.getheight();
  Utils utils;

  switch (angle) {
    case 90: {
      Picture *new_pic = new Picture(origHeight, origWidth);
      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic->setpixel(origHeight - y - 1, x, pic.getpixel(x, y));
        }
      }

      delete it->second;
      it->second = new_pic;
      cout << "Rotated " << filename << " by 90deg clockwise successfully." << endl;
      break;
    }

    case 180: {
      Picture *new_pic = new Picture(origWidth, origHeight);
      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic->setpixel(origWidth - x - 1, origHeight - y - 1, pic.getpixel(x, y));
        }
      }

      delete it->second;
      it->second = new_pic;
      cout << "Rotated " << filename << " by 180deg clockwise successfully." << endl;
      break;
    }

    case 270: {
      Picture *new_pic = new Picture(origHeight, origWidth);
      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic->setpixel(y, origWidth - x - 1, pic.getpixel(x, y));
        }
      }

      delete it->second;
      it->second = new_pic;
      cout << "Rotated " << filename << " by 270deg clockwise successfully." << endl;
      break;
    }
  }
}

void PicLibrary::flipVH(char axis, string filename) {

  map<string, Picture*>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = *(it->second);
  int origWidth = pic.getwidth();
  int origHeight = pic.getheight();

  Picture *new_pic = new Picture(origWidth, origHeight);

  switch (axis) {
    case 'H':

      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic->setpixel(origWidth - x - 1, y, pic.getpixel(x, y));
        }
      }

      delete it->second;
      it->second = new_pic;
      cout << "Rotated " << filename << " horizontally successfully." << endl;

      break;
    case 'V':

      for (int x = 0; x < origWidth; x++) {
        for (int y = 0; y < origHeight; y++) {
          new_pic->setpixel(x, origHeight - y - 1, pic.getpixel(x, y));
        }
      }

      delete it->second;
      it->second = new_pic;
      cout << "Flipped " << filename << " vertically successfully." << endl;
      break;
  }
}

/* Concurrent data structures and functions for blur */
struct job_args {
  Picture *pic;
  Picture *new_pic;
  int start_x;
  int end_x;
  int start_y;
  int end_y;
  int orig_height;
  int orig_width;
};

static void *threaded_blur(void *args) {
  job_args *job_info = (job_args*) args;
  Picture *pic = job_info->pic;
  Picture *new_pic = job_info->new_pic;
  int start_x = job_info->start_x;
  int end_x = job_info->end_x;
  int start_y = job_info->start_y;
  int end_y = job_info->end_y;
  int orig_height = job_info->orig_height;
  int orig_width = job_info->orig_width;

  for (int x = start_x; x <= end_x; x++) {
    for (int y = start_y; y <= end_y; y++) {
      if (x != 0 && x != orig_width - 1 && y != 0 && y != orig_height - 1) {
        int sumR = 0;
        int sumG = 0;
        int sumB = 0;
        for (int i = -1; i < 2; i++) {
          for (int j = -1; j < 2; j++) {
            Colour pixel = pic->getpixel(x + i, y + j);
            sumR += pixel.getred();
            sumG += pixel.getgreen();
            sumB += pixel.getblue();
          }
        }
        new_pic->setpixel(x, y, Colour(sumR / 9, sumG / 9, sumB / 9));
      } else {
        new_pic->setpixel(x, y, pic->getpixel(x, y));
      }
    }
  }
}

/* Optimal implementation utilising knowledge of number of cores */
// void PicLibrary::blur(string filename) {
//   map<string, Picture*>::iterator it = picStore.find(filename);
//
//   if (it == picStore.end()) {
//     cout << "Picture: " << filename << " doesn't exist" << endl;
//     return;
//   }
//
//   Picture pic = *(it->second);
//   int origWidth = pic.getwidth();
//   int origHeight = pic.getheight();
//
//   Picture *new_pic = new Picture(origWidth, origHeight);
//   vector<pthread_t> threads_list;
//
//   unsigned cores = std::thread::hardware_concurrency();
//   cout << "Number of cores detected: " << cores << endl;
//   int section_size = origWidth / cores;
//   for (int i = 0; i < cores - 1; i++) {
//     job_args *args = new job_args;
//     args->start_x = i * section_size;
//     args->end_x = (i + 1) * section_size - 1;
//     args->start_y = 0;
//     args->end_y = origHeight - 1;
//     args->orig_height = origHeight;
//     args->orig_width = origWidth;
//     args->pic = it->second;
//     args->new_pic = new_pic;
//
//     pthread_t running_thread;
//     if (!(pthread_create(&running_thread, NULL, &threaded_blur, args) == 0)) {
//       cout << "Failed to create thread!" << endl;
//     } else {
//       threads_list.push_back(running_thread);
//     }
//   }
//
//   job_args *args = new job_args;
//   args->start_x = (cores - 1) * section_size;
//   args->end_x = ((cores - 1) + 1) * section_size - 1 + (origWidth % cores);
//   args->start_y = 0;
//   args->end_y = origHeight - 1;
//   args->orig_height = origHeight;
//   args->orig_width = origWidth;
//   args->pic = it->second;
//   args->new_pic = new_pic;
//
//   pthread_t running_thread;
//   if (!(pthread_create(&running_thread, NULL, &threaded_blur, args) == 0)) {
//     cout << "Failed to create thread!" << endl;
//   } else {
//     threads_list.push_back(running_thread);
//   }
//
//
//   cout << "Joining all threads.." << endl;
//   for(std::vector<pthread_t>::iterator i = threads_list.begin(); i != threads_list.end(); ++i) {
//     pthread_join(*i, NULL);
//   }
//
//   delete it->second;
//   it->second = new_pic;
//   cout << "Blurred " << filename << " successfully." << endl;
// }

/* column-by-column threaded implementation of blurring images */
// void PicLibrary::blur(string filename) {
//   map<string, Picture*>::iterator it = picStore.find(filename);
//
//   if (it == picStore.end()) {
//     cout << "Picture: " << filename << " doesn't exist" << endl;
//     return;
//   }
//
//   Picture pic = *(it->second);
//   int origWidth = pic.getwidth();
//   int origHeight = pic.getheight();
//
//   Picture *new_pic = new Picture(origWidth, origHeight);
//   vector<pthread_t> threads_list;
//
//   for (int x = 0; x < origWidth; x++) {
//     job_args *args = new job_args;
//     args->start_x = x;
//     args->end_x = x;
//     args->start_y = 0;
//     args->end_y = origHeight - 1;
//     args->orig_height = origHeight;
//     args->orig_width = origWidth;
//     args->pic = it->second;
//     args->new_pic = new_pic;
//
//     pthread_t running_thread;
//     if (!(pthread_create(&running_thread, NULL, &threaded_blur, args) == 0)) {
//       cout << "Failed to create thread!" << endl;
//     } else {
//       threads_list.push_back(running_thread);
//     }
//   }
//
//   cout << "Joining all threads.." << endl;
//   for(std::vector<pthread_t>::iterator i = threads_list.begin(); i != threads_list.end(); ++i) {
//     pthread_join(*i, NULL);
//   }
//
//   delete it->second;
//   it->second = new_pic;
//   cout << "Blurred " << filename << " successfully." << endl;
// }

/* row-by-row threaded implementation of blurring images */
// void PicLibrary::blur(string filename) {
//   map<string, Picture*>::iterator it = picStore.find(filename);
//
//   if (it == picStore.end()) {
//     cout << "Picture: " << filename << " doesn't exist" << endl;
//     return;
//   }
//
//   Picture pic = *(it->second);
//   int origWidth = pic.getwidth();
//   int origHeight = pic.getheight();
//
//   Picture *new_pic = new Picture(origWidth, origHeight);
//   vector<pthread_t> threads_list;
//
//   for (int y = 0; y < origHeight; y++) {
//     job_args *args = new job_args;
//     args->start_x = 0;
//     args->end_x = origWidth - 1;
//     args->start_y = y;
//     args->end_y = y;
//     args->orig_height = origHeight;
//     args->orig_width = origWidth;
//     args->pic = it->second;
//     args->new_pic = new_pic;
//
//     pthread_t running_thread;
//     if (!(pthread_create(&running_thread, NULL, &threaded_blur, args) == 0)) {
//       cout << "Failed to create thread!" << endl;
//     } else {
//       threads_list.push_back(running_thread);
//     }
//   }
//
//   cout << "Joining all threads.." << endl;
//   for(std::vector<pthread_t>::iterator i = threads_list.begin(); i != threads_list.end(); ++i) {
//     pthread_join(*i, NULL);
//   }
//
//   delete it->second;
//   it->second = new_pic;
//   cout << "Blurred " << filename << " successfully." << endl;
// }

/* pixel-by-pixel threaded implementation for blurring images */
// void PicLibrary::blur(string filename) {
//   map<string, Picture*>::iterator it = picStore.find(filename);
//
//   if (it == picStore.end()) {
//     cout << "Picture: " << filename << " doesn't exist" << endl;
//     return;
//   }
//
//   Picture pic = *(it->second);
//   int origWidth = pic.getwidth();
//   int origHeight = pic.getheight();
//
//   Picture *new_pic = new Picture(origWidth, origHeight);
//   vector<pthread_t> threads_list;
//
//   for (int x = 0; x < origWidth; x++) {
//     for (int y = 0; y < origHeight; y++) {
//       job_args *args = new job_args;
//       args->start_x = x;
//       args->end_x = x;
//       args->start_y = y;
//       args->end_y = y;
//       args->orig_height = origHeight;
//       args->orig_width = origWidth;
//       args->pic = it->second;
//       args->new_pic = new_pic;
//
//       pthread_t running_thread;
//       if (!(pthread_create(&running_thread, NULL, &threaded_blur, args) == 0)) {
//         cout << "Failed to create thread!" << endl;
//       } else {
//         threads_list.push_back(running_thread);
//       }
//     }
//   }
//
//   cout << "Joining all threads.." << endl;
//   for(std::vector<pthread_t>::iterator i = threads_list.begin(); i != threads_list.end(); ++i) {
//     pthread_join(*i, NULL);
//   }
//
//   delete it->second;
//   it->second = new_pic;
//   cout << "Blurred " << filename << " successfully." << endl;
// }

/* Non-concurrent version of blur */
void PicLibrary::blur(string filename) {
  map<string, Picture*>::iterator it = picStore.find(filename);

  if (it == picStore.end()) {
    cout << "Picture: " << filename << " doesn't exist" << endl;
    return;
  }

  Picture pic = *(it->second);
  int origWidth = pic.getwidth();
  int origHeight = pic.getheight();

  Picture *new_pic = new Picture(origWidth, origHeight);

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
      new_pic->setpixel(x, y, Colour(sumR / 9, sumG / 9, sumB / 9));
    }

    for (int x = 0; x < origWidth; x++) {
      new_pic->setpixel(x, 0, pic.getpixel(x, 0));
      new_pic->setpixel(x, origHeight - 1, pic.getpixel(x, origHeight - 1));
    }

    for (int y = 1; y < origHeight - 1; y++) {
      new_pic->setpixel(0, y, pic.getpixel(0, y));
      new_pic->setpixel(origWidth - 1, y, pic.getpixel(origWidth - 1, y));
    }
  }

  delete it->second;
  it->second = new_pic;
  cout << "Blurred " << filename << " successfully." << endl;
}

void PicLibrary::free() {
  map<string, Picture*>::iterator it;
  for (it = picStore.begin(); it != picStore.end(); it++) {
    cout << "PicLib free 1" << endl;
    delete it->second;
    cout << "PicLib free 2" << endl;
  }
}
