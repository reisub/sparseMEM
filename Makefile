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

test1: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence1.fasta test_cases/query1.fasta 2 10

test2: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence2.fasta test_cases/query2.fasta 2 10

test3: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence3.fasta test_cases/query3.fasta 2 10

test4: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence4.fasta test_cases/query4.fasta 2 10

test5: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence5.fasta test_cases/query5.fasta 2 10

test6: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence6.fasta test_cases/query6.fasta 2 10

test7: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence7.fasta test_cases/query7.fasta 2 10

test8: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence8.fasta test_cases/query8.fasta 2 10

test9: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence9.fasta test_cases/query9.fasta 2 10

test10: $(EXECUTABLE)
	./$(EXECUTABLE) test_cases/sequence10.fasta test_cases/query10.fasta 2 10

unit: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

clean:
	-rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean
