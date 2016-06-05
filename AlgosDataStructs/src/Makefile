include ../Makefile.inc

OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))

all: $(LIB_STATIC) $(LIB_DYN) 

$(LIB_STATIC): $(OBJS) 
	$(DIR_GUARD) $(LIB_PATH)
	$(AR) $(ARFLAGS) $@ $^ $(OBJS)

$(LIB_DYN): $(OBJS)
	$(DIR_GUARD) $(LIB_PATH)
	$(CXX) $(CXXFLAGS) $(LDLIBFLAGS) -o $@ $^
	ln -fs $(LIB_DYN) $(LIB_PATH)/lib$(LIB_NAME).so

clean:
	$(RM) *.o $(LIB_STATIC) $(LIB_PATH)/*.so*
