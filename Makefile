PROGS = find-all
OBJS = find-all.o
PMEM_FLAGS = -lpmem -lpmemobj -lpthread

CXX = g++

all: $(PROGS)
.PHONY : all

%.o : %.cpp
	$(CXX) -std=c++11 -O2 -c $< -o $@

find-all: find-all.o
	$(CXX) $< -O2 -o $@

clean:
	$(RM) $(OBJS) $(PROGS)

