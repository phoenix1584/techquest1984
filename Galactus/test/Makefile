include ../Makefile.inc

all: $(TESTS)
LIBS += -L$(BASE_PATH)/src -lgalactus
.cc:
	$(RULE_TESTS)

clean:
	$(DEL_FILE) $(OBJS) $(TESTS) 
