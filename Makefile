CFLAGS=-c -Wall -Wextra -std=c++11
LDFLAGS=
SOURCES=main.cc sa_is.cc fasta_parser.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CXX) $(CFLAGS) $< -o $@

test: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/simple.txt test_cases/query.txt 1 2

clean:
	-rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean
