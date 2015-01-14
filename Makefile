CFLAGS=-g -c -Wall -Wextra -std=c++11
LDFLAGS=
SOURCES=main.cc sa_is.cc fasta_parser.cc search.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=main
TEST_SOURCES=search_test.cc
TEST_OBJECTS=$(TEST_SOURCES:.cc=.o)
TEST_EXECUTABLE=search_test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(OBJECTS)
	$(CXX) $(LDFLAGS) $(TEST_OBJECTS) -o $@

.cc.o:
	$(CXX) $(CFLAGS) $< -o $@

test: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/mississippi.fasta test_cases/mississippi_query.fasta 1 2

unit: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

ref: mummer
	./mummer -maxmatch -b -n -threads 3 test_cases/query.txt test_cases/simple.txt -k 1 -l 30

clean:
	-rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean
