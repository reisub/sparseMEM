CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cc sa_is.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CXX) $(CFLAGS) $< -o $@

test: $(EXECUTABLE)
	./$(EXECUTABLE) 10 test_cases/simple1_a.txt test_cases/simple1_b.txt

clean:
	-rm $(OBJECTS) $(EXECUTABLE)
