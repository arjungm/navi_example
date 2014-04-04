CC := g++ # This is the main compiler
CFLAGS := -g -Wall

INCLUDES := -Iinclude
LFLAGS := -Llib -lboost_program_options

SRCDIR := src

TARGET := bin/navigate

#Environment.o: $(SRCDIR)/Environment.cpp
#	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Environment.cpp

Nav2DGraphState.o: $(SRCDIR)/Nav2DGraphState.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Nav2DGraphState.cpp

Nav2DGraph.o: $(SRCDIR)/Nav2DGraph.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Nav2DGraph.cpp

main.o: $(SRCDIR)/main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/main.cpp

all: Nav2DGraphState.o Nav2DGraph.o main.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) Nav2DGraphState.o Nav2DGraph.o main.o $(LFLAGS)

#end
