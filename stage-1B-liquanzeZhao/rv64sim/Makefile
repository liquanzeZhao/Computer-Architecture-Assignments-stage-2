CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++11 -Wall -pedantic
LDFLAGS=-g
LDLIBS=

SRCS=rv64sim.cpp commands.cpp memory.cpp processor.cpp processorF.cpp   
OBJS=$(subst .cpp,.o,$(SRCS))

all: rv64sim

rv64sim: $(OBJS)
	$(CXX) $(LDFLAGS) -o rv64sim $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	rm -f ./.depend
	$(RM) $(OBJS)
	$(RM) rv64sim

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
