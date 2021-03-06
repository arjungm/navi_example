CC := g++ # This is the main compiler
CFLAGS := -g -Wall

INCLUDES := -Iinclude
LFLAGS := -Llib -lboost_program_options -lboost_filesystem -lboost_system

SRCDIR := src

TARGET := navigate

Cell.o: $(SRCDIR)/Cell.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Cell.cpp

Environment.o: $(SRCDIR)/Environment.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Environment.cpp

Graph.o: $(SRCDIR)/Graph.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Graph.cpp

Planner.o: $(SRCDIR)/Planner.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Planner.cpp

main.o: $(SRCDIR)/main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/main.cpp

all: Cell.o Environment.o Graph.o Planner.o main.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) Cell.o Environment.o Graph.o Planner.o main.o $(LFLAGS)

#end
