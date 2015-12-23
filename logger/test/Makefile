CC = gcc
CXX = g++
CPPFLAGS = $(DEFINES) $(INC)
CMPLR_FLAGS_CMN = -Wall -Wextra -Werror -Wno-unused-parameter
CXXFLAGS = -Wno-unused-variable $(CMPLR_FLAGS_CMN)
CFLAGS = -ansi -pedantic $(CMPLR_FLAGS_CMN)

PLTFM_ROOT_DIR = ../../../platforms

PLTFM_INC_DIR = $(PLTFM_ROOT_DIR)/inc
PLTFM_SRC_DIR = $(PLTFM_ROOT_DIR)/src

OSAL_INC_DIR = $(PLTFM_ROOT_DIR)/cygwin/inc
OSAL_SRC_DIR = $(PLTFM_ROOT_DIR)/cygwin/src

TEST_UTILS_INC_DIR = ../../../utils/test_utils/inc
TEST_UTILS_SRC_DIR = ../../../utils/test_utils/src

# Make sure that the GTEST_ROOT environment variable is set correctly.
GTEST_ROOT := $(subst \,/,$(GTEST_ROOT)) # If the path is of Windows-style convert to POSIX-style
GTEST_ROOT := $(strip $(GTEST_ROOT))
GTEST_LIB = $(GTEST_ROOT)/make
GTEST_INC = $(GTEST_ROOT)/include

VPATH = ../src:../test:$(LOGGER_SRC_DIR):$(TEST_UTILS_SRC_DIR)\
	:$(PLTFM_SRC_DIR):$(OSAL_SRC_DIR)
INC = -I../inc -I../test \
	-I$(TEST_UTILS_INC_DIR) -I$(GTEST_INC) \
	-I$(PLTFM_INC_DIR) -I$(OSAL_INC_DIR)
DEFINES = -DLOGGER -DLOG_TO_STDOUT -DLOGGER_LEVEL=6

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


#-------------------------------------------
# Module under Test - Logger
#-------------------------------------------
MUT = 
# Test Files with testcases
TF_TC = logger_tests.cpp
# Modules the MUT is dependent on
MUT_DPD = 
ifeq ($(findstring -DLOGGER, $(DEFINES)), -DLOGGER)
	MUT_DPD += logger.c
endif

SOURCES = $(MUT) $(MUT_DPD) $(PLTFM_FILES) $(TF_TC) $(TF_CMN)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

logger_tests: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

logger_tests_debug: CMPLR_FLAGS_CMN += -g3 -O0
logger_tests_debug: $(OBJECTS)
	$(CXX) -o $@ $^ $(GTEST_LIB)/gtest_main.a

logger_tests_dpd: $(OBJECTS:.o=.d)

all: logger_tests

.PHONY: clean clean_full
clean:
	@rm -f *.o *.exe

clean_full:
	@rm -f *.o *.exe *.d
