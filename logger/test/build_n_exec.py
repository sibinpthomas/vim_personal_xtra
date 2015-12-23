import os

# Choose the feature to be tested (i.e. (f)eature (u)nder (t)est)
fut = ('logger',
       )[0]

# Choose the build type
build_type = ('_tests',        # Test the FUT
              '_tests_debug',  # Debug the FUT with GDB
              '_tests_dpd',    # Generate the dependencies of each source file
              )[0]

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

retval = os.system("make clean_full")
retval = os.system("make " + fut_btype)
if 0 == retval and repeat_n > 0:
    cargs_str = ' '.join(cargs)
    cargs_str = ' ' * (cargs_str is not '') + cargs_str
    if build_type == '_tests':
        os.system(fut_btype + ".exe" + cargs_str)
        for i in xrange(repeat_n - 1):
            os.system(fut_btype + ".exe" + cargs_str)
    elif build_type == '_tests_debug':
        os.system("gdb --silent --args " + fut_btype + ".exe" + cargs_str)

retval = os.system("make clean")
