CC := g++ # This is the main compiler
CFLAGS := -g -Wall

INCLUDES := -Iinclude
LFLAGS := -Llib -lboost_program_options

SRCDIR := src

TARGET := bin/navigate

Environment.o: $(SRCDIR)/Environment.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Environment.cpp

Graph.o: $(SRCDIR)/Graph.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Graph.cpp

Planner.o: $(SRCDIR)/Planner.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Planner.cpp

main.o: $(SRCDIR)/main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/main.cpp

all: Environment.o Graph.o Planner.o main.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) Environment.o Graph.o Planner.o main.o $(LFLAGS)

#end
