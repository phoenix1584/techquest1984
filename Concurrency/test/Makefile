include ../Makefile.inc

OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))
TARGETS := $(patsubst %.cc,%,$(wildcard *.cc))
CXXFLAGS += -DBOOST_TEST_DYN_LINK   
all:$(TARGETS) 
$(TARGETS):$(OBJS)
	$(DIR_GUARD) $(BIN_PATH)
	$(CXX) $(CXXFLAGS) -o  $(BIN_PATH)/$@ $< $(LIB_STATIC) $(LIBS)  

clean:
	$(RM) *.o $(BIN_PATH)/*
