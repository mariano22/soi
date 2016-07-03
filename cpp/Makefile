appname := main

CXX := g++ 
CXXFLAGS := -lpthread -std=c++11

srcfiles := comunic.cpp dispatcher.cpp fdManage.cpp globalFiles.cpp ids.cpp localConections.cpp localFiles.cpp mainWorker.cpp main.cpp mensaje.cpp openedFiles.cpp parser.cpp procTask.cpp realFs.cpp sockaux.cpp task.cpp tokenControl.cpp tokenQueues.cpp
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) -o $(appname) $(objects)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f *~ .depend

include .depend
