appname := main

CXX := g++ 
CXXFLAGS := -std=c++11 -DDEBUG_FLAG

srcfiles := main.cpp mensaje.cpp openedFiles.cpp parser.cpp procTask.cpp realFs.cpp sockaux.cpp task.cpp tokenControl.cpp tokenQueues.cpp dispatcher.cpp comunic.cpp fdManage.cpp globalFiles.cpp ids.cpp localConections.cpp localFiles.cpp mainWorker.cpp
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) -o $(appname) $(objects) -lpthread

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend; 

clean:
	rm -f $(objects)
	rm -f *~ .depend
	rm -f $(appname)

include .depend
