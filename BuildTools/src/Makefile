include ../Makefile.inc

OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))

all: $(LIB_STATIC) $(LIB_DYN) 

$(LIB_STATIC): $(OBJS) 
	$(AR) $(ARFLAGS) $@ $^ $(OBJS)

$(LIB_DYN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDLIBFLAGS) -o $@ $^
	ln -fs $(LIB_DYN) $(LIB_PATH)/lib$(LIB_NAME).so

clean:
	$(RM) *.o $(LIB_STATIC) $(LIB_PATH)/*.so* 
