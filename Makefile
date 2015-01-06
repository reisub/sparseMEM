CFLAGS=-c -Wall -Wextra
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
	./$(EXECUTABLE) test_cases/simple.txt

clean:
	-rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean
