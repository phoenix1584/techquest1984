include ../Makefile.inc

CXXFLAGS += -DBOOST_TEST_DYN_LINK   
.PHONY : all test clean .cc

all: $(TESTS) 
	
.cc: 
	$(RULE_TESTS)	

clean:
	$(DEL_FILE) $(TESTS) $(OBJS)
