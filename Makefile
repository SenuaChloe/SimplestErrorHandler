CXX=g++
CXXFLAGS=-I. --std=c++20
LIBS=-lgtest -lgtest_main -lpthread
DEPS = ErrorHandler.h
TEST_DIR = ./tests
OBJ = $(TEST_DIR)/test0.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LIBS)

default:
	$(info By default, there is no need to compile, as it is a header-only library.)
	$(info To compile the tests, run `make test`)

tests: tests/test0

tests/test0: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJ)
	rm -f tests/test0
