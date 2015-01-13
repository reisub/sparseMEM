CFLAGS=-g -c -Wall -Wextra -std=c++11
LDFLAGS=
SOURCES=main.cc sa_is.cc fasta_parser.cc search.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CXX) $(CFLAGS) $< -o $@

test: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/afumig.fasta test_cases/query.fa 1 30
	./$(EXECUTABLE) test_cases/anidulans.fasta test_cases/q_anidulans 50 70

correct:
  ./mummer -maxmatch -b -n -threads 3 -k 50 -l 70 test_cases/anidulans.fasta test_cases/q_anidulans

clean:
	-rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean
