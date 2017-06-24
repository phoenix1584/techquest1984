include $(PWD)/paths.env

CXX= g++ -std=c++11
DEBUG= -g
CXXFLAGS= -pipe $(DEBUG) -Wall -O2 -fPIC 
LD_LIB_FLAGS= -fPIC -shared -rdynamic -Wl,-soname,$(BASE_LIB_NAME).so
DEL_FILE= rm -f
AR= ar
ARFLAGS= cqs

INC_BOOST= -I$(BOOST_INC_PATH)
LIB_BOOST= -L $(BASE_PATH)/extern/lib/
LIB_BOOST_TEST= -DBOOST_TEST_DYN_LINK -DBOOST_LOG_DYN_LINK $(LIB_BOOST) -lboost_unit_test_framework

LIBS =
INC_PATH = $(INC_BOOST) -I$(BASE_PATH)/include

# Target Directories
BIN_PATH= $(BASE_PATH)/bin

# Generic Rules

SRCS=$(wildcard *.cc)

# object files
OBJS=$(patsubst %.cc,%.o,$(SRCS))
%.o:%.cc
	$(CXX) $(CXXFLAGS) $(INC_PATH) -c $< 

RULE_TESTS = \
    $(CXX) $(CXXFLAGS) $(INC_PATH)  $< -o $@ $(LIB_BOOST_TEST) $(APP_LIB)     

# Modules as application
RULE_APP = \
    $(CXX) $(CXXFLAGS) $(INC_PATH) -o $(BASE_PATH)/$(APP_NAME) main.cpp $(OBJS) $(LIBS)

# Modules as dynamic library 
RULE_LIB_DYN = \
    	$(CXX) $(CXXFLAGS) $(LD_LIB_FLAGS) -o $@ $^ $(LIBS) ; \
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
    ./$$t_case --log_level=test_suite --log_format=XML --log_sink=$(UFO_SVN_BASE_PATH)/$$t_case.xml --result_code=no --report_level=detailed --report_format=XML   ;    \
done

#Rule to clean files
BASE_LIB_NAME=dummy_as_default
APP_NAME=dummy
RULE_CLEAN_FILES = \
	$(DEL_FILE) $(LIB_DYN) $(LIB_STATIC) $(OBJS) $(BASE_LIB_NAME).so* $(BIN_PATH)/$(APP_NAME)
