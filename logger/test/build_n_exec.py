import os

# Choose the feature to be tested (i.e. (f)eature (u)nder (t)est)
fut = ('logger',
       )[0]

# Choose the build type
build_type = ('_tests',        # Test the FUT
              '_tests_debug',  # Debug the FUT with GDB
              '_tests_dpd',    # Generate the dependencies of each source file
              )[0]

# Should the Logger be enabled?
enable_logger = (False, True)[1]
if enable_logger:
    # Set Logging level
    logger_level = 0  # 0 to 6 are valid values.
    # The Log statements are to be printed on 'Stdout' or the 'Log File'?
    print_to_stdout = (False, True)[1]

# Run the tests 'n' number of times
# Choose 0 (zero) to just build with no subsequent testing.
repeat_n = 1

# Command line arguments to be passed to the executable
cargs = ["--gtest_filter=*",
         # "--gtest_shuffle",
         # "--gtest_repeat=10",
         ]

###################################################
fut_btype = fut + build_type
def_str = "DEFINES=\""
if enable_logger:
    def_str += "-DLOGGER"
    def_str += " -DLOGGER_LEVEL=" + str(logger_level)
    if print_to_stdout:
        def_str += " -DLOG_TO_STDOUT"
def_str += "\""

retval = os.system("make clean_full")
retval = os.system("make " + def_str + " " + fut_btype)
if 0 == retval and repeat_n > 0:
    cargs_str = ' '.join(cargs)
    cargs_str = ' ' * (cargs_str is not '') + cargs_str
    if build_type == '_tests':
        os.system(fut_btype + ".exe" + cargs_str)
        for i in xrange(repeat_n - 1):
            os.system(fut_btype + ".exe" + cargs_str)
    elif build_type == '_tests_debug':
        os.system("gdb --silent --args " + fut_btype + ".exe" + cargs_str)
elif retval != 0:
    print "The command - 'make " + def_str + " " + fut_btype + "'" +\
        " returned with a value " + str(retval)

retval = os.system("make clean")
