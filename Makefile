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
<<<<<<< HEAD
	./$(EXECUTABLE) test_cases/simple.txt test_cases/query.txt 2 3
=======
>>>>>>> 758b2c2fbf5a56e3e942d2c3c524ea7d767a1d78
	./$(EXECUTABLE) test_cases/afumig.fasta test_cases/query.fa 3 20

clean:
	-rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean
