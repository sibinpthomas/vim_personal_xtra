import sys
from itertools import product
from collections import namedtuple

Parameter = namedtuple("Parameter", ['bad', 'good'])


def gen_all_err_param_combos(param_list, prefix, suffix, write2file):
    """
    Generate all error throwing combinations of parameters of a function.

    'param_list' - List of parameters.
    'prefix' - String to be printed before the parameter combinations.
    'suffix' - String to be printed after the parameter combinations.
    'write2file' - 0 : stdout, 1 : File write, 2 : File append
    """

    # If, param1 = Parameter(["bad"], ["good"])
    #     param2 = Parameter(["bad"], ["good"])
    #
    # Product = [(["bad"], ["bad"]),
    #            (["bad"], ["good"]),
    #            (["good"], ["bad"]),
    #            (["good"], ["good"])]
    iterable = product(*param_list)
    combo_count = 0

    # Pulling one item ahead of loop so as to avoid the last product i.e.
    # combination of all "good" parameters.
    prev_bg_prod = next(iterable)

    # Setting output destination
    if write2file == 0:
        op_dest = sys.stdout
    elif write2file == 1:
        fp = open("file.cpp", "w")
        op_dest = fp
    elif write2file == 2:
        fp = open("file.cpp", "a")
        op_dest = fp
    else:
        return -1

    for bg_prod in iterable:
        # Operating on previous bad-good product.
        for sub_prod in product(*prev_bg_prod):
            prod = [str(x) if type(x) is not str else x for x in sub_prod]
            op_dest.write(prefix + ", ".join(prod) + suffix)
            op_dest.write("\n")
            combo_count += 1

        prev_bg_prod = bg_prod

    if write2file:
        fp.close()
    return combo_count

if __name__ == "__main__":
    param_list = []
    param1 = Parameter([], ['BOOL_TRUE', 'BOOL_FALSE'])
    param_list.append(param1)

    param2 = Parameter([512, 1023], [1024, 1234, 65000])
    param_list.append(param2)

    param3 = Parameter(['0'], ['10'])
    param_list.append(param3)

    param4 = Parameter([], ['NULL', '\"127.0.0.1\"'])
    param_list.append(param4)

    # API_RESULT sockets_init(BOOL_T , UINT16 , UINT16 , DECL_CONST CHAR* );
    prefix = "    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init("
    suffix = "));"
    write2file = 1  # 0 - stdout, 1 - File write, 2 - File append

    count = gen_all_err_param_combos(param_list, prefix, suffix, write2file)
    print count
