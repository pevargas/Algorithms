#Which compiler?
CC = g++
#Which flags?
CFLAGS = -Wall
#Which files?
SOURCES = pvargas-assign4.cpp
OBJECTS = $(SOURCES: .cpp = .o)
#Name of program
EXECUTABLE = poly

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -rf *o $(EXECUTABLE)

# Obligatory UNIX inside joke
love:
	@echo "not war?"