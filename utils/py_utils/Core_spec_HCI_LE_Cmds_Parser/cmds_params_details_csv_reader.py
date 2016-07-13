import sys
import csv


def generate_cmd_param_range_lookup(pdrd, stream=sys.stdout):
    """
    Generate a list of ranges for HCI LE Command parameters of type
    UINT8 or UINT16.
    """

    s_b = "/* Ordered listing of each command's parameter's range check. */\n" +\
          "DECL_STATIC DECL_CONST "
    s_e = " cmd_prm_range[] =\n" +\
          "{"

    uint8_range_list = []
    uint16_range_list = []
    uint8_arr_range_vfunc_list = []
    new_cmd = False

    params_size_8 = 0
    params_size_16 = 0
    params_array = 0

    for row in pdrd:
        if len(row) == 2:
            new_cmd = True
        elif len(row) > 2:
            if new_cmd:
                s_e += "\n   "
                new_cmd = False

            if row[1] == '1':
                if row[2]:
                    tmp = row[2].split('to')
                    range = (int(tmp[0], 0), int(tmp[1], 0))

                    if range is not (0x00, 0xFF) and range not in uint8_range_list:
                        uint8_range_list.append(range)
                    s_e += " " + str(uint8_range_list.index(range) + 1) + ","
                    params_size_8 += 1
                else:
                    s_e += " 0,"
            elif row[1] == '2':
                if row[2]:
                    tmp = row[2].split('to')
                    range = (int(tmp[0], 0), int(tmp[1], 0))

                    if range is not (0x0000, 0xFFFF) and range not in uint16_range_list:
                        uint16_range_list.append(range)
                    s_e += " " + str(uint16_range_list.index(range) + 1) + ","
                    params_size_16 += 1
                else:
                    s_e += " 0,"
            else:
                if row[2]:
                    range_vfunc = row[2]
                    if range_vfunc not in uint8_arr_range_vfunc_list:
                        uint8_arr_range_vfunc_list.append(range_vfunc)
                    s_e += " " + str(uint8_arr_range_vfunc_list.index(range_vfunc) + 1) + ","
                    params_array += 1
                else:
                    s_e += " 0,"

    s_e += "\n};"

    l = "typedef struct\n" +\
        "{\n" +\
        "    UINT8 min;\n" +\
        "    UINT8 max;\n" +\
        "}PARAM_RANGE_8;\n" +\
        "\n" +\
        "typedef struct\n" +\
        "{\n" +\
        "    UINT16 min;\n" +\
        "    UINT16 max;\n" +\
        "}PARAM_RANGE_16;\n" +\
        "\n" +\
        "typedef BOOL_T (*vfunc)(UINT8 *, UINT16 );\n" +\
        "\n" +\
        "/* Parameter ranges list - UINT8. */\n" +\
        "DECL_STATIC DECL_CONST PARAM_RANGE_8 cmd_prm_range_list_8[] =\n" +\
        "{"
    for range in uint8_range_list:
        l += ("\n    {0x%02X, 0x%02X}," % (range[0], range[1]))
    l += "\n};\n" +\
        "\n" +\
        "/* Parameter ranges list - UINT16. */\n" +\
        "DECL_STATIC DECL_CONST PARAM_RANGE_16 cmd_prm_range_list_16[] =\n" +\
        "{"
    for range in uint16_range_list:
        l += ("\n    {0x%04X, 0x%04X}," % (range[0], range[1]))
    l += "\n};" +\
        "\n"
    for vfunc in uint8_arr_range_vfunc_list:
        l += "\nBOOL_T " + vfunc + "(UINT8* arr, UINT16 opcode)" +\
             "\n{" +\
             "\n    return BOOL_TRUE;" +\
             "\n}\n"
    l += "\n" +\
        "/* UINT8 array Parameter verification functions list. */\n" +\
        "DECL_STATIC DECL_CONST vfunc cmd_prm_vfunc_list_uint8_array[] =\n" +\
        "{"
    for vfunc in uint8_arr_range_vfunc_list:
        l += "\n    " + vfunc + ","
    l += "\n};\n\n"

    greatest = (params_size_8, params_size_16)[params_size_16 > params_size_8]
    greatest = (greatest, params_array)[params_array > greatest]
    if greatest < 0x100:
        typ = "UINT8"
    elif greatest < 0x10000:
        typ = "UINT16"
    else:
        raise

    s = s_b + typ + s_e
    stream.write(l + s)


def generate_cmd_param_default_val_lookup(pdrd, stream=sys.stdout):
    """
    Generate a list of deafult values for HCI LE Command parameters of type
    UINT8 or UINT16.
    """

    s_b = "/* Ordered listing of each command's UINT8/UINT16 parameter's default value. */\n" +\
          "DECL_STATIC DECL_CONST "
    s_e = " cmd_prm_dflt_val[] =\n" +\
          "{"

    uint8_deflt_list = []
    uint16_deflt_list = []
    uint8_arr_deflt_pool = ""
    uint8_arr_deflt_list = []
    new_cmd = False

    params_size_8 = 0
    params_size_16 = 0
    params_array = 0

    for row in pdrd:
        if len(row) == 2:
            new_cmd = True
        elif len(row) > 2:
            if new_cmd:
                s_e += "\n   "
                new_cmd = False

            if row[1] == '1':
                dflt_val = 0
                if row[3]:
                    dflt_val = int(row[3], 0)

                if dflt_val not in uint8_deflt_list:
                    uint8_deflt_list.append(dflt_val)
                s_e += " " + str(uint8_deflt_list.index(dflt_val) + 1) + ","
                params_size_8 += 1
            elif row[1] == '2':
                dflt_val = 0
                if row[3]:
                    dflt_val = int(row[3], 0)

                if dflt_val not in uint16_deflt_list:
                    uint16_deflt_list.append(dflt_val)
                s_e += " " + str(uint16_deflt_list.index(dflt_val) + 1) + ","
                params_size_16 += 1
            else:
                dflt_val = row[3]
                if dflt_val not in uint8_arr_deflt_pool:
                    if uint8_arr_deflt_pool:
                        uint8_arr_deflt_pool += " "
                    uint8_arr_deflt_pool += dflt_val
                pool_idx = uint8_arr_deflt_pool.index(dflt_val)/5
                if pool_idx not in uint8_arr_deflt_list:
                    uint8_arr_deflt_list.append(pool_idx)
                s_e += " " + str(uint8_arr_deflt_list.index(pool_idx) + 1) + ","
                params_array += 1

    s_e += "\n};"

    l = "" +\
        "/* Parameter ranges list - UINT8. */\n" +\
        "DECL_STATIC DECL_CONST UINT8 cmd_prm_def_val_list_8[] =\n" +\
        "{\n" +\
        "   "
    for dflt_val in uint8_deflt_list:
        l += (" 0x%02X," % dflt_val)
    l += "\n};\n" +\
        "\n" +\
        "/* Parameter ranges list - UINT16. */\n" +\
        "DECL_STATIC DECL_CONST UINT16 cmd_prm_def_val_list_16[] =\n" +\
        "{\n" +\
        "   "
    for dflt_val in uint16_deflt_list:
        l += (" 0x%04X," % dflt_val)
    l += "\n};\n" +\
        "\n" +\
        "/* Parameter arrays default value. */\n" +\
        "DECL_STATIC DECL_CONST UINT8 cmd_prm_def_val_array[] =\n" +\
        "{"
    octet_list = uint8_arr_deflt_pool.split(' ')
    num_octets = 8
    for i in xrange(0, len(octet_list), num_octets):
        l += "\n    " + ", ".join(octet_list[i:i+num_octets])
        l += ','
    l += "\n};\n" +\
        "\n" +\
        "/* Parameter ranges list - Arrays. */\n" +\
        "DECL_STATIC DECL_CONST UINT8* cmd_prm_def_val_list_arr[] =\n" +\
        "{"
    for idx in uint8_arr_deflt_list:
        l += "\n    &cmd_prm_def_val_array[" + str(idx) + "],"
    l += "\n};\n\n"

    greatest = (params_size_8, params_size_16)[params_size_16 > params_size_8]
    greatest = (greatest, params_array)[params_array > greatest]
    if greatest < 0x100:
        typ = "UINT8"
    elif greatest < 0x10000:
        typ = "UINT16"
    else:
        raise

    s = s_b + typ + s_e
    stream.write(l + s)


if __name__ == "__main__":
    with open('cmd_params_details.csv', 'rb') as csvfile, open("parsed_hci_cmds.txt", "w") as f:
        # Param details reader.
        pdrd = csv.reader(csvfile, delimiter='|', skipinitialspace=True)

        # generate_cmd_param_default_val_lookup(pdrd, f)
        generate_cmd_param_range_lookup(pdrd, f)
