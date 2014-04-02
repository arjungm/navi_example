CC := g++ # This is the main compiler
CFLAGS := -g -Wall

INCLUDES := -Iinclude
LFLAGS := -Llib -lboost_program_options

SRCDIR := src

TARGET := bin/navigate

Environment.o: $(SRCDIR)/Environment.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/Environment.cpp

main.o: $(SRCDIR)/main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -c $(SRCDIR)/main.cpp

all: Environment.o main.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) Environment.o main.o $(LFLAGS)

clean:
	rm -rf *o hello

#end
