CC = gcc
CXX = g++
CPPFLAGS = $(DEFINES) $(INC)
CMPLR_FLAGS_CMN = -Wall -Wextra -Werror -Wno-unused-parameter
CXXFLAGS = -Wno-unused-variable $(CMPLR_FLAGS_CMN)
CFLAGS = -ansi -pedantic $(CMPLR_FLAGS_CMN)

ROOT_DIR = ../../..
PLTFM_ROOT_DIR = $(ROOT_DIR)/platforms

PLTFM_INC_DIR = $(PLTFM_ROOT_DIR)/inc
PLTFM_SRC_DIR = $(PLTFM_ROOT_DIR)/src

OSAL_INC_DIR = $(PLTFM_ROOT_DIR)/cygwin/inc
OSAL_SRC_DIR = $(PLTFM_ROOT_DIR)/cygwin/src

LOGGER_INC_DIR = $(ROOT_DIR)/utils/logger/inc
LOGGER_SRC_DIR = $(ROOT_DIR)/utils/logger/src

TEST_UTILS_INC_DIR = $(ROOT_DIR)/utils/test_utils/inc
TEST_UTILS_SRC_DIR = $(ROOT_DIR)/utils/test_utils/src

TOP_INC_DIR = $(ROOT_DIR)/top/inc
TOP_SRC_DIR = $(ROOT_DIR)/top/src

# Make sure that the GTEST_ROOT environment variable is set correctly.
GTEST_ROOT := $(subst \,/,$(GTEST_ROOT)) # If the path is of Windows-style convert to POSIX-style
GTEST_ROOT := $(strip $(GTEST_ROOT))
GTEST_LIB = $(GTEST_ROOT)/make
GTEST_INC = $(GTEST_ROOT)/include

VPATH = ../src:../test:$(LOGGER_SRC_DIR):$(TEST_UTILS_SRC_DIR)\
	:$(PLTFM_SRC_DIR):$(OSAL_SRC_DIR)
INC = -I../inc -I../test \
	  -I$(TOP_INC_DIR) \
	  -I$(PLTFM_INC_DIR) -I$(OSAL_INC_DIR) -I$(LOGGER_INC_DIR)\
	  -I$(TEST_UTILS_INC_DIR) -I$(GTEST_INC)

DEFINES = #-DLOGGER -DLOGGER_LEVEL=0 -DLOG_TO_STDOUT
DEFINES += #-DSOCKETS_LOG_ENABLE
DEFINES += #-DWINDOWS

# Platform files
PLTFM_FILES = pltfm_auxiliary.c

# Test Files Common
TF_CMN = test_utils.c


# Rules to generate dependency files
#-----------------------------------
%.d : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MM -MF $@ $<
	@sed -i 's/ \\//g' $@
	@sed -i 's/\(\.[hc]p*\) /\1\n /g' $@
	@sed -i 's/^ \([^ ]\)/    \1/g' $@
	@sed -i '/^.*platforms.*$$/d' $@
	@sed -i '/^.*logger.*$$/d' $@

%.d : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $@ $<
	@sed -i 's/ \\//g' $@
	@sed -i 's/\(\.[hc]p*\) /\1\n /g' $@
	@sed -i 's/^ \([^ ]\)/    \1/g' $@
	@sed -i '/^.*platforms.*$$/d' $@
	@sed -i '/^.*logger.*$$/d' $@
	@sed -i '/^.*gtest.*$$/d' $@


# Build Cscope cross-reference file
#---------------------------------
cscope:
	cd ../ && cscope -b -s ./src $(subst -I../, -I, $(INC))

cscope_test:
	cd ../test && find .. -name "*.cpp" -o -name "*.c" > cscope.files && cscope -b $(INC) && rm -f cscope.files


#----------------------------
# Module under Test - Sockets
#----------------------------
MUT = sockets.c
# Test Files with testcases
TF_TC = sockets_tests.cpp
# Modules the MUT is dependent on
MUT_DPD = 
ifeq ($(findstring -DLOGGER, $(DEFINES)), -DLOGGER)
	MUT_DPD += logger.c
endif

SOURCES = $(MUT) $(MUT_DPD) $(PLTFM_FILES) $(TF_TC) $(TF_CMN)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

sockets_tests: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

sockets_tests_debug: CMPLR_FLAGS_CMN += -g3 -O0
sockets_tests_debug: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

sockets_tests_dpd: $(OBJECTS:.o=.d)


#--------------------------------------
# Module under Test - Sockets White Box
#--------------------------------------
MUT = 
# Test Files with testcases
TF_TC = sockets_wbox_tests.cpp
# Modules the MUT is dependent on
MUT_DPD =
ifeq ($(findstring -DLOGGER, $(DEFINES)), -DLOGGER)
	MUT_DPD += logger.c
endif

SOURCES = $(MUT) $(MUT_DPD) $(PLTFM_FILES) $(TF_TC) $(TF_CMN)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

sockets_wbox_tests: CMPLR_FLAGS_CMN += -Wno-unused-function
sockets_wbox_tests: INC += -I../src
sockets_wbox_tests: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

sockets_wbox_tests_debug: CMPLR_FLAGS_CMN += -g3 -O0 -Wno-unused-function
sockets_wbox_tests_debug: INC += -I../src
sockets_wbox_tests_debug: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

sockets_wbox_tests_dpd: $(OBJECTS:.o=.d)


#--------------------------------------
# Module under Test - Socket Test Utils
#--------------------------------------
MUT = 
# Test Files with testcases
TF_TC = sockets_test_utils_tests.cpp
# Modules the MUT is dependent on
MUT_DPD =
ifeq ($(findstring -DLOGGER, $(DEFINES)), -DLOGGER)
	MUT_DPD += logger.c
endif

SOURCES = $(MUT) $(MUT_DPD) $(PLTFM_FILES) $(TF_TC) $(TF_CMN)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

socket_test_utils_tests: CMPLR_FLAGS_CMN += -Wno-unused-function
socket_test_utils_tests: INC += -I../src
socket_test_utils_tests: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

socket_test_utils_tests_debug: CMPLR_FLAGS_CMN += -g3 -O0 -Wno-unused-function
socket_test_utils_tests_debug: INC += -I../src
socket_test_utils_tests_debug: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

socket_test_utils_tests_dpd: $(OBJECTS:.o=.d)


#---------------------------------------------------
# Module under Test - Sockets Communication (Server)
#---------------------------------------------------
MUT = sockets.c
# Test Files with testcases
TF_TC = sockets_comm_tests_server.cpp
# Modules the MUT is dependent on
MUT_DPD = socket_test_utils.c
ifeq ($(findstring -DLOGGER, $(DEFINES)), -DLOGGER)
	MUT_DPD += logger.c
endif

SOURCES = $(MUT) $(MUT_DPD) $(PLTFM_FILES) $(TF_TC) $(TF_CMN)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

sockets_comm_server_tests: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

sockets_comm_server_tests_debug: CMPLR_FLAGS_CMN += -g3 -O0
sockets_comm_server_tests_debug: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

sockets_comm_server_tests_dpd: $(OBJECTS:.o=.d)


#---------------------------------------------------
# Module under Test - Sockets Communication (Client)
#---------------------------------------------------
MUT = sockets.c
# Test Files with testcases
TF_TC = sockets_comm_tests_client.cpp
# Modules the MUT is dependent on
MUT_DPD = socket_test_utils.c
ifeq ($(findstring -DLOGGER, $(DEFINES)), -DLOGGER)
	MUT_DPD += logger.c
endif

SOURCES = $(MUT) $(MUT_DPD) $(PLTFM_FILES) $(TF_TC) $(TF_CMN)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

sockets_comm_client_tests: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

sockets_comm_client_tests_debug: CMPLR_FLAGS_CMN += -g3 -O0
sockets_comm_client_tests_debug: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

sockets_comm_client_tests_dpd: $(OBJECTS:.o=.d)

all: sockets_tests sockets_wbox_tests socket_test_utils_tests

.PHONY: clean clean_full
clean:
	@rm -f *.o *.exe

clean_full:
	@rm -f *.o *.exe *.d
