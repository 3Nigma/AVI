CPP := g++
CXX_PATH = /home/enigma/Projects/externe/cxxtest/
LDFLAGS := -llua5.1

PROJ := monavi
CXXOUT := trun
CXX_RUNNER_SRC := test_runner.cxx
OBJS := main.o \
exceptions.o \
settings.o
TSTS := settings_tests.cxx

./%.cxx : ./%.hpp
	$(CXX_PATH)/bin/cxxtestgen --part --error-printer -o $*.cxx $<

./%.o : ./%.cpp
	$(CPP) -Wall -std=c++0x -c $< -o $@

.PHONY: all tests clean

all: $(PROJ) tests

run_tests: tests
	./$(CXXOUT)

$(PROJ): $(OBJS)
	$(CPP) -Wall $(OBJS) -o $@ $(LDFLAGS)
tests: $(TSTS) $(CXX_RUNNER_SRC)
	$(CPP) -std=c++0x -o $(CXXOUT) -I$(CXX_PATH) $(subst main.o ,,$(OBJS)) $(CXX_RUNNER_SRC) $(TSTS) $(LDFLAGS)
$(CXX_RUNNER_SRC):
	$(CXX_PATH)/bin/cxxtestgen --root --error-printer -o $@

clean:
	rm -f $(PROJ) $(OBJS) $(TSTS) $(CXXOUT) test_runner.cxx