include ../Makefile.inc

all: $(TESTS)
APP_LIB += -L $(BASE_PATH)/src/ -lgalactus
.cc:
	$(RULE_TESTS)

clean:
	$(DEL_FILE) $(OBJS) $(TESTS) 
