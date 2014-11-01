CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

test: $(EXECUTABLE)
	./$(EXECUTABLE) 10 test_cases/simple1_a.txt test_cases/simple1_b.txt

clean:
	-rm $(OBJECTS) $(EXECUTABLE)
