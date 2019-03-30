
MODE=release
CC=clang++
CCFLAGS=-c -Wall
RELEASE_FLAGS=-O2
#DBG_FLAGS=-g -DDEBUG -l gtest -l gtest_main -l pthreads -L lib/
DBG_FLAGS=-g -DDEBUG
SOURCES=src/main.cpp src/dataset.cpp src/cryptanalysis.cpp src/getoptpp/*.cpp
TESTS=src/dataset_utest.cpp
OBJECTS=*.o

cryptan: $(OBJECTS)
	mkdir -p bin/
	$(CC) $(OBJECTS) -o bin/$@
	rm -f *.o

$(OBJECTS):
ifeq ($(MODE), release)
	$(CC) $(CCFLAGS) $(RELEASE_FLAGS) $(SOURCES)
else
	$(CC) $(CCFLAGS) $(DBG_FLAGS) $(SOURCES)
endif

clean:
	rm -rf bin/*
	rm -f *.o
