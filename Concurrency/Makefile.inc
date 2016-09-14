#Release Versions
MAJOR_VERSION= 0
MINOR_VERSION= 0
RELEASE_VERSION= 1

#Paths variables
BASE_PATH=$(PWD)
INC_PATH= -I $(PWD)/include/ -I $(HOME)/CodeBase/boost_1_61_0 -I $(HOME)/CodeBase/zeromq-4.1.5/include -I $(HOME)/CodeBase/cppzmq/ -I $(HOME)/CodeBase/zguide/examples/C++/
LIB_PATH=$(PWD)/lib/
BIN_PATH=$(PWD)/bin/
DIR_GUARD=@mkdir -p
LIBS= -lpthread -L$(HOME)/CodeBase/boost_1_61_0/stage/lib -lboost_serialization -L$(HOME)/CodeBase/boost_1_61_0/stage/lib -lboost_unit_test_framework -L$(HOME)/CodeBase/zeromq-4.1.5/.libs/ -lzmq 

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

ifeq "$(GCC_VER_CMP48)" "1"
    CXXFLAGS += -std=c++11
else
    @echo "Compiling with c++98" 
endif

####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
		$(CXX) -c $(CXXFLAGS) $(INC_PATH) -o $@ $< 

.cc.o:
		$(CXX) -c $(CXXFLAGS) $(INC_PATH) -o $@ $< 

.cxx.o:
		$(CXX) -c $(CXXFLAGS) $(INC_PATH) -o $@ $< 

.C.o:
		$(CXX) -c $(CXXFLAGS) $(INC_PATH) -o $@ $< 

.c.o:
		$(CC)  -c $(CFLAGS) $(INC_PATH) -o $@ $< 
