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
	./$(EXECUTABLE) test_cases/mississippi.fasta test_cases/mississippi_query.fasta 1 3

test2: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/afumig_small.fasta test_cases/query.txt 2 30

test3: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/anidulans.fasta test_cases/q_anidulans 2 30

test4: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/human21_short.fasta test_cases/human21_short_query.fasta 1 20

unit: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

ref2: mummer
	./mummer -maxmatch -b -n -threads 3 test_cases/query.txt test_cases/simple.txt -k 2 -l 30
	
ref3: mummer
	./mummer -maxmatch -b -n -threads 3 test_cases/q_anidulans test_cases/anidulans.fasta -k 2 -l 30
	
ref4: mummer
	./mummer -maxmatch -b -n -threads 3 test_cases/human21_short_query.fasta test_cases/human21_short.fasta -k 1 -l 20

clean:
	-rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean
