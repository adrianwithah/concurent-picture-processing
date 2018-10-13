
picture_lib: Main.o Utils.o Colour.o Picture.o PicLibrary.o Command.o PicThread.o CommandSyncer.o
	g++ `pkg-config --cflags opencv` Main.o Utils.o Colour.o Picture.o PicLibrary.o Command.o PicThread.o CommandSyncer.o `pkg-config --libs opencv` -o picture_lib -lpthread

Utils.o: Utils.hpp Utils.cpp

Colour.o: Colour.hpp Colour.cpp

Picture.o: Utils.hpp Picture.hpp Picture.cpp

PicLibrary.o: Utils.hpp PicLibrary.hpp PicLibrary.cpp

Main.o: Main.cpp Utils.hpp Colour.hpp Picture.hpp PicLibrary.hpp Command.hpp PicThread.hpp

PicThread.o: PicThread.cpp

Command.o: Command.cpp

CommandSyncer.o: CommandSyncer.cpp

%.o: %.cpp
	g++ -std=c++11 -c $< -lpthread

clean:
	rm -rf picture_lib *.o

.PHONY: clean
