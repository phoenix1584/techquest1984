include ../Makefile.inc

OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))
TARGETS := $(patsubst %.cc,%,$(wildcard *.cc))


all:$(TARGETS) 
$(TARGETS):$(OBJS)
	$(CXX) $(CXXFLAGS) $< -o $(BIN_PATH)/$@ $(LIB_STATIC)

clean:
	$(RM) *.o $(BIN_PATH)/*
