#Release Versions
MAJOR_VERSION= 0
MINOR_VERSION= 0
RELEASE_VERSION= 1

#Paths variables
BASE_PATH=$(PWD)
INC_PATH= -I $(PWD)/include/ -I $(HOME)/CodeBase/boost_1_61_0 -I $(HOME)/CodeBase/zeromq-4.1.5/include -I $(HOME)/CodeBase/cppzmq/ -I $(HOME)/CodeBase/zguide/examples/C++/ -I $(HOME)/CodeBase/poco-1.7.3/Util/include -I $(HOME)/CodeBase/poco-1.7.3/Foundation/include
LIB_PATH=$(PWD)/lib/
BIN_PATH=$(PWD)/bin/
DIR_GUARD=@mkdir -p
LIBS= -lpthread -L$(HOME)/CodeBase/boost_1_61_0/stage/lib -lboost_serialization -L$(HOME)/CodeBase/boost_1_61_0/stage/lib -lboost_unit_test_framework -L$(HOME)/CodeBase/zeromq-4.1.5/.libs/ -lzmq -L$(HOME)/CodeBase/poco-1.7.3/lib/Linux/x86_64 -lPocoFoundation -L$(HOME)/CodeBase/poco-1.7.3/lib/Linux/x86_64 -lPocoJSON -L$(HOME)/CodeBase/poco-1.7.3/lib/Linux/x86_64 -lPocoUtil -L$(HOME)/CodeBase/poco-1.7.3/lib/Linux/x86_64 -lPocoXML 

# Header Files
SYS_INC= $(CODE_SYS_ROOT)/usr/include
INC_POCO= -I $(POCO_BASE_DIR)/Foundation/include -I$(POCO_BASE_DIR)/Util/include -I$(POCO_BASE_DIR)/Net/include
INC_ZMQ= -I $(ZMQ_BASE_DIR)/include
INC_PROTO_BUFF= -I $(PROTO_BUFF_BASE_DIR)/src/google/protobuf/
INC_BOOST=-I$(BOOST_BASE_DIR)/
INC_PATH=  I ../include $(INC_BOOST) $(INC_POCO) $(INC_ZMQ)
 
# Libraries
#FIXME : Custom paths are architecture specfic locations and boost dir locations are not generic. Installing in /usr/local becomes necessary for sanity of setup.
SYS_LIBS= /usr/lib
LIB_BOOST= -L$(BOOST_BASE_DIR)/stage/lib -lboost_thread -L$(BOOST_BASE_DIR)/stage/lib -lboost_system  -L$(BOOST_BASE_DIR)/stage/lib -lboost_serialization  -L$(BOOST_BASE_DIR)/stage/lib -lboost_filesystem  -L$(BOOST_BASE_DIR)/stage/lib -lboost_timer
LIB_BOOST_TEST= -DBOOST_TEST_DYN_LINK -DBOOST_LOG_DYN_LINK -L$(BOOST_BASE_DIR)/stage/lib -lboost_unit_test_framework   
LIB_ZMQ=-L$(ZMQ_BASE_DIR)/.libs -lzmq
LIB_PROTO_BUFF=-L$(PROTO_BUFF_BASE_DIR)/src/.libs -lprotobuf-lite
LIB_POCO=-L$(POCO_BASE_DIR)/lib/Linux/$(TARGET_PLATFORM) -lPocoFoundation -L$(POCO_BASE_DIR)/lib/Linux/$(TARGET_PLATFORM) -lPocoUtil -L$(POCO_BASE_DIR)/lib/Linux/$(TARGET_PLATFORM) -lPocoXML -L$(POCO_BASE_DIR)/lib/Linux/$(TARGET_PLATFORM) -lPocoJSON -L$(POCO_BASE_DIR)/lib/Linux/$(TARGET_PLATFORM) -lPocoNet
LIB_SYS= -pthread -lrt -lcrypto

# Refs : GCC_COMPAT
GCC_VER_CMP48 :=$(shell (echo `gcc -dumpversion | cut -f1-2 -d.` \>= 4.8 | bc))
GCC_VERSION :=$(shell gcc --version | grep ^gcc | sed 's/^.* //g')
ARCH:=$(shell uname -m)
CC=gcc
CXX=g++
CFLAGS = -pipe -Wall -O2
CXXFLAGS = -pipe -Wall -O2 -Wno-unused-function
DEBUG = -g
FPIC = -fpic
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
GPROF_FLAGS= -pg
SAVE_TEMPS= -save-temps
RM= rm -f
ARFLAGS= cqs
AR = ar

# Library Details
LIB_NAME=concurrency
LIB_STATIC=$(LIB_PATH)/lib$(LIB_NAME).a
LIB_DYN=$(LIB_PATH)/lib$(LIB_NAME).so.$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION)
LDLIBFLAGS=$(FPIC) -shared -rdynamic -Wl,-soname,lib$(LIB_NAME).so

# Add flags for gcc5 and higher versions to check for c++14 and and c++1z
ifeq "$(GCC_VER_CMP48)" "1"
    CXXFLAGS += -std=c++11
else
    @echo "Compiling with c++98" 
endif

####### Implicit rules

# Generic Rules

SRCS=$(wildcard *.cc)

# object files
OBJS=$(patsubst %.cc,%.o,$(SRCS))
%.o:%.cc
	$(CXX) $(CXXFLAGS) $(INC_PATH) -c $< 

RULE_TESTS = \
    $(CXX) $(CXXFLAGS) $(INC_PATH) $(INC_POCO) $(INC_ZMQ) $(INC_PROTO_BUFF)  $< -o $@ $(LIB_CODE_SCHEMA) $(LIB_CODE) $(LIB_BOOST_TEST) $(LIBS)   

# Modules as application
RULE_APP = \
    $(CXX) $(CXXFLAGS) $(INC_PATH) -o $(CODE_BIN_PATH)/$(APP_NAME) main.cpp $(OBJS) $(LIBS)

# Modules as dynamic library 
RULE_LIB_DYN = \
    	$(CXX) $(CXXFLAGS) $(LD_LIB_FLAGS) $< -o $@ $(LIBS); \
    	ln -fs $(LIB_DYN) $(BASE_LIB_NAME).so

# Modules as static library
RULE_LIB_STATIC = \
	$(AR) $(ARFLAGS) $@ $^ $(OBJS)

# Execute the test suite
# TODO: Options for confirming test cases successful completion.
#      -- Redirect the output to temporary files and check for failure related keywords
#         generated by boost framework. Check boost test framework reference for more details.
#      -- Use asserts in test case for stopping the test cases from proceeding individually.  

# Individual Executables names 
TESTS=$(SRCS:.cc=) 

RULE_TEST_SUITE = \
    for t_case in $(TESTS); do \
        ./$$t_case --log_level=test_suite --log_format=XML --log_sink=$(CODE_BASE_DIR)/$$t_case.xml --result_code=no --report_level=detailed --report_format=XML   ;	\
	done

# Auto generation of header files using Flatbuffer Schema files.
# Possible eliminate the version controlled storage of auto generated files
# to ensure all new changes are considered by the code-gen tools.
FBS=$(wildcard *.fbs)
RULE_FBS_HEADER = \
    for fbs in $(FBS); do \
        flatc --cpp $$fbs ;	\
	done

# Auto generation of header files using Protocol Buffer Schema files.
# Possible eliminate the version controlled storage of auto generated files
# to ensure all new changes are considered by the code-gen tools.
PBS=$(wildcard *.proto)
PBS_SRC=$(patsubst %.proto,%.pb.cc,$(PBS))
PBS_OBJS=$(patsubst %.cc,%.o,$(PBS_SRC))
RULE_PBS_AUTOGEN = \
    for pbs in $(PBS); do \
        protoc --cpp_out=. $$pbs ;	\
	done ; \
    for pbs_src in $(PBS_SRC); do \
        $(CXX) $(CXXFLAGS) $(INC_PROTO_BUFF) -c $$pbs_src ; \
    done ;\
	$(AR) $(ARFLAGS) $@ $^ $(PBS_OBJS)
     
