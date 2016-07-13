import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
from logger import logger
from pdf_hci_cmds_parser_fsm import pdf_hci_cmds_parser_fsm, Event,\
    Abst_FSM_Excptn, excptn
import ConfigParser
import re
from collections import OrderedDict

lgr = logger()


class PdfHciCmdsParserConfig(object):
    lcaps = "a-z"
    ucaps = "A-Z"
    hyphen = "-"
    alpha = lcaps + ucaps
    L = alpha + "_"
    D = "0-9"
    X = D + "a-fA-F"
    WS = "\t "
    NL = "\r\n"
    empty_line = "^\s*$"

    def __init__(self, config_file):
        # Read config file.
        ini_parser = ConfigParser.ConfigParser()
        ini_parser.read(config_file)

        self.section_beg_patterns = set()
        for item in ini_parser.items('regex'):
            setattr(self, item[0], item[1])
            if 'hdr' in item[0]:
                self.section_beg_patterns.add(item[1])

        for item in ini_parser.items('integer'):
            setattr(self, item[0], int(item[1]))


class PdfHciParser(object):
    def __init__(self, config_file):
        self.pc = PdfHciCmdsParserConfig(config_file)

    def parse_pdf_text(self, f):
        # Instantiating FSM object
        pdf_hci_parser_fsm_obj = pdf_hci_cmds_parser_fsm(self.pc)

        line_num = 0

        for line in f.readlines():
            line = line.strip(self.pc.NL)
            line_num += 1

            if re.match(self.pc.trivial_reject, line):
                # Trivially rejecting empty lines and page header and footer
                # text instead of going through the whole FSM shebang before
                # ignoring this.
                pass

            else:
                event = Event("Ev_New_Line", (line, line_num))

                try:
                    pdf_hci_parser_fsm_obj.handle_event(event)
                except Abst_FSM_Excptn as ae:
                    lgr.log_error(ae.value)
                    if ae.value[:len(excptn["Unexpected Event"])] !=\
                            excptn["Unexpected Event"]:
                        raise

                if pdf_hci_parser_fsm_obj.finito:
                    break
        else:
            event = Event("Ev_EOF", None)

            try:
                pdf_hci_parser_fsm_obj.handle_event(event)
            except Abst_FSM_Excptn as ae:
                lgr.log_error(ae.value)
                if ae.value[:len(excptn["Unexpected Event"])] !=\
                        excptn["Unexpected Event"]:
                    raise

        if pdf_hci_parser_fsm_obj.error_string:
            return None
        else:
            # If there was no error during the parsing then return the full
            # HCI Commands dictionary.
            return pdf_hci_parser_fsm_obj.hci_cmds


def display_hci_cmds_details(hci_cmds_dom, stream=sys.stdout):
    """
    HCI Commands DOM Schema -
    -------------------------

    hci_cmds_dom =
        {  # <Ordered Dictionary>

            HCI_CMD_01:
                {  # <Dictionary>

                    OCF: <integer>
                    CMD_PARAMS:
                        {  # <Ordered Dictionary>

                            param_01:
                            {  # <Dictionary>

                                SIZE: <integer> (number of octets)
                                BITS: <integer> (num bits relevant in octets, optional)
                                RANGE: <tuple(min <integer>, max <integer>)> (optional)
                                DEFAULT: - (optional)
                            }
                            param_02:
                            {
                            }
                        }
                    RET_PARAMS:
                        {  # <Ordered Dictionary>

                            param_01:
                            {  # <Dictionary>

                                SIZE: <integer> (number of octets)
                                BITS: <integer> (num bits relevant in octets, optional)
                                RANGE: <tuple(min <integer>, max <integer>)> (optional)
                                DEFAULT: - (optional)
                            }
                            param_02:
                            {
                            }
                        }
                    DESCRIPTION: <string>
                    GEN_EVENTS: <string>
                }

            HCI_CMD_02:
                {
                }
        }
    """

    s = ""
    s += "\nHCI Command DOM -"
    for i, cmd in enumerate(hci_cmds_dom):
        s += "\n" + str(i+1) + ". " + cmd
        s += ("\n\t OCF = 0x%04X" % hci_cmds_dom[cmd]['OCF'])
        s += "\n\t Command Parameters -"
        for j, cmd_param in enumerate(hci_cmds_dom[cmd]['CMD_PARAMS']):
            s += "\n\t\t" + str(j+1) + ". " + cmd_param
            s += "\n\t\t\t Size: %d" % hci_cmds_dom[cmd]['CMD_PARAMS'][cmd_param]['SIZE']
            if hci_cmds_dom[cmd]['CMD_PARAMS'][cmd_param]['BITS']:
                s += "\n\t\t\t Bits: %d" % hci_cmds_dom[cmd]['CMD_PARAMS'][cmd_param]['BITS']
        s += "\n"
        s += "\n\t Return Parameters -"
        for j, ret_param in enumerate(hci_cmds_dom[cmd]['RET_PARAMS']):
            s += "\n\t\t" + str(j+1) + ". " + ret_param
            s += "\n\t\t\t Size: %d" % hci_cmds_dom[cmd]['RET_PARAMS'][ret_param]['SIZE']
            if hci_cmds_dom[cmd]['RET_PARAMS'][ret_param]['BITS']:
                s += "\n\t\t\t Bits: %d" % hci_cmds_dom[cmd]['RET_PARAMS'][ret_param]['BITS']
        s += "\n"
        s += "\n"

    stream.write(s)


def display_cmd_params_len(hci_cmds_dom, stream=sys.stdout):
    """
    Prints array of total size of HCI Command parameters.
    Missing HCI commands are given a default size of 0xFF (no holes).
    """
    prev_cmd = None
    for cmd in hci_cmds_dom:
        cmd_params_len = 0
        for cmd_param in hci_cmds_dom[cmd]['CMD_PARAMS']:
            cmd_params_len += hci_cmds_dom[cmd]['CMD_PARAMS'][cmd_param]['SIZE']

        if prev_cmd and hci_cmds_dom[cmd]['OCF'] > (hci_cmds_dom[prev_cmd]['OCF'] + 1):
            for ocf in xrange(hci_cmds_dom[prev_cmd]['OCF'] + 1, hci_cmds_dom[cmd]['OCF']):
                stream.write("{ 0xFF },\n")
        stream.write("{ " + str(cmd_params_len) + " },\n")
        prev_cmd = cmd


def display_ret_params_len(hci_cmds_dom, stream=sys.stdout):
    """
    Prints array of total size of HCI Command return parameters.
    Missing HCI commands are given a default size of 0xFF (no holes).
    """
    prev_cmd = None
    for cmd in hci_cmds_dom:
        ret_params_len = 0
        for ret_param in hci_cmds_dom[cmd]['RET_PARAMS']:
            ret_params_len += hci_cmds_dom[cmd]['RET_PARAMS'][ret_param]['SIZE']

        if prev_cmd and hci_cmds_dom[cmd]['OCF'] > (hci_cmds_dom[prev_cmd]['OCF'] + 1):
            for ocf in xrange(hci_cmds_dom[prev_cmd]['OCF'] + 1, hci_cmds_dom[cmd]['OCF']):
                stream.write("{ 0xFF },\n")
        stream.write("{ " + str(ret_params_len) + " },\n")
        prev_cmd = cmd


def display_diff_param_len(hci_cmds_dom, stream=sys.stdout):
    """
    Prints set of Command and Return parameter sizes.
    """
    cmd_params_len = set()
    for cmd in hci_cmds_dom:
        for cmd_param in hci_cmds_dom[cmd]['CMD_PARAMS']:
            cmd_params_len.add(hci_cmds_dom[cmd]['CMD_PARAMS'][cmd_param]['SIZE'])

    ret_params_len = set()
    for cmd in hci_cmds_dom:
        for ret_param in hci_cmds_dom[cmd]['RET_PARAMS']:
            ret_params_len.add(hci_cmds_dom[cmd]['RET_PARAMS'][ret_param]['SIZE'])

    print "The set of command parameter lengths - ",
    print sorted(cmd_params_len)
    print "The set of return parameter lengths  - ",
    print sorted(ret_params_len)


def generate_cmd_param_lookup_tbl(hci_cmds_dom, stream=sys.stdout):
    """
    Prints lookup table to find data type of a command parameter.
    """
    tab = '    '

    types_list_dic = {1: 'UINT8_TYPE', 2: 'UINT16_TYPE',
                      5: 'ARR_UINT8_5_TYPE', 6: 'ARR_UINT8_6_TYPE',
                      8: 'ARR_UINT8_8_TYPE', 16: 'ARR_UINT8_16_TYPE',
                      31: 'ARR_UINT8_31_TYPE', 64: 'ARR_UINT8_64_TYPE'}

    types_list = "/* Set of the data types of the HCI LE command parameters.\n" +\
                 " *\n" +\
                 " * These data types are encoded as an incremental list of numbers,\n" +\
                 " * starting with 0 with an increment size of 1.\n" +\
                 " * These values are used as indices to the 'type_len_list' array (below)." +\
                 " */\n"
    types_len_list = "/* Length of the data types in the above set. */\n"
    types_len_list += "DECL_STATIC DECL_CONST UINT8 type_len_list[] = {\n"
    for i, size in enumerate(sorted(types_list_dic.keys())):
        define_str = "#define " + types_list_dic[size]
        ws = " " + (" " * (43 - len(define_str)))
        types_list += define_str + ws + str(i) + "\n"
        types_len_list += tab + str(size) + ", /* " + types_list_dic[size] + " */\n"
    types_len_list += "};"

    break_num = 8
    meta_data_dump = "/* Ordered listing of each command's each parameter's meta-data. */\n"
    meta_data_dump += "DECL_STATIC DECL_CONST UINT8 cmd_prm_meta_data[] =\n{"

    meta_data_loc  = "/* Start location of a HCI LE command's parameters meta-data in the above array.\n"
    meta_data_loc += " * '0' is used to indicate 'no command parameters'.\n"
    meta_data_loc += " * As a result the locations in this array are offset by '1'. */\n"
    meta_data_loc += "DECL_STATIC DECL_CONST UINT16 cmd_prm_meta_data_loc[] =\n{"
    prev_cmd = None
    cmd_count = 0
    param_loc = 1
    param_loc_start = 0

    for cmd in hci_cmds_dom:
        if param_loc > param_loc_start:
            meta_data_dump += "\n" + tab[:-1]
        param_loc_start = param_loc
        for param in hci_cmds_dom[cmd]['CMD_PARAMS']:
            if hci_cmds_dom[cmd]['CMD_PARAMS'][param]['SIZE'] in types_list_dic:
                size = hci_cmds_dom[cmd]['CMD_PARAMS'][param]['SIZE']
                meta_data_dump += " " + types_list_dic[size] + ','
                param_loc += 1
            else:
                raise

        if prev_cmd and hci_cmds_dom[cmd]['OCF'] > (hci_cmds_dom[prev_cmd]['OCF'] + 1):
            for ocf in xrange(hci_cmds_dom[prev_cmd]['OCF'] + 1, hci_cmds_dom[cmd]['OCF']):
                if (cmd_count % break_num) == 0:
                    meta_data_loc += "\n" + tab
                meta_data_loc += str(0) + ", "
                cmd_count += 1

        if cmd_count % break_num == 0:
            meta_data_loc += "\n" + tab

        if param_loc > param_loc_start:
            meta_data_loc += str(param_loc_start) + ", "
        else:
            meta_data_loc += str(0) + ", "
        prev_cmd = cmd
        cmd_count += 1

    meta_data_dump = meta_data_dump.rstrip(" \n")
    meta_data_dump += "\n};\n"
    meta_data_loc += "\n};\n"

    stream.write(types_list)
    stream.write("\n")
    stream.write(types_len_list)
    stream.write("\n\n")
    stream.write(meta_data_dump)
    stream.write("\n")
    stream.write(meta_data_loc)


def generate_cmd_funcs(hci_cmds_dom, stream=sys.stdout):
    """
    Generate HCI LE Command functions which make use of the
    HCI LE common command API.
    """

    types_list_dic = {1: 'UINT8_TYPE', 2: 'UINT16_TYPE',
                      5: 'ARR_UINT8_5_TYPE', 6: 'ARR_UINT8_6_TYPE',
                      8: 'ARR_UINT8_8_TYPE', 16: 'ARR_UINT8_16_TYPE',
                      31: 'ARR_UINT8_31_TYPE', 64: 'ARR_UINT8_64_TYPE'}

    tab = '    '

    s = ""
    for cmd in hci_cmds_dom:
        func = "API_RESULT " + cmd.replace(' ', '_').lower() + "("
        s += func
        for param in hci_cmds_dom[cmd]['CMD_PARAMS']:
            size = hci_cmds_dom[cmd]['CMD_PARAMS'][param]['SIZE']
            param = param.lower()
            if size in types_list_dic:
                typ = types_list_dic[size]
                if typ.startswith('ARR'):
                    s += "UINT8 (*" + param + ")[XXX_SIZE]" + ",\n"
                else:
                    s += typ.split('_')[0] + " " + param + ",\n"
                s += ' ' * len(func)
        else:
            if not s.endswith("("):
                s = s[:-(2 + len(func))]
            else:
                s += "void"
        s += ")"
        s += "\n{"
        s += "\n" + tab + "HCI_LE_CMD_PARAMS hci_le_cmd_param;"
        s += "\n"
        for i, param in enumerate(hci_cmds_dom[cmd]['CMD_PARAMS']):
            size = hci_cmds_dom[cmd]['CMD_PARAMS'][param]['SIZE']
            typ = types_list_dic[size]
            s += "\n" + tab + "hci_le_cmd_param[" + str(i) + "] = " +\
                 ('&', '')[typ.startswith('ARR')] + param.lower() + ";"
        else:
            s += "\n" + tab + "hci_le_cmd_param[" + str(i+1) + "] = NULL;"

        s += "\n"
        s += "\n" + tab + "return hci_le_common_api(" + cmd.replace(' ', '_').upper() + "_OPCODE" + ", hci_le_cmd_param);"
        s += "\n}\n\n"

    stream.write(s)


def generate_cmd_param_name_lookup(hci_cmds_dom, stream=sys.stdout):
    """
    Generate a list of HCI LE Command parameters names.
    """

    s_b = "/* Ordered listing of each command's each parameter's name. */\n" +\
          "DECL_STATIC DECL_CONST "
    s_e = " cmd_prm_name[] =\n" +\
          "{"

    param_list = []
    for cmd in hci_cmds_dom:
        if len(hci_cmds_dom[cmd]['CMD_PARAMS']) > 0:
            s_e += "\n   "
        for param in hci_cmds_dom[cmd]['CMD_PARAMS']:
            if param not in param_list:
                param_list.append(param)
                param_loc = len(param_list) - 1
            else:
                param_loc = param_list.index(param)
            s_e += " " + str(param_loc) + ","

    s_e += "\n};\n"

    l = "/* Parameter names list. */\n" +\
        "DECL_STATIC DECL_CONST CHAR* cmd_prm_name_list[] =\n" +\
        "{"
    for param in param_list:
        l += "\n    \"" + param.replace('_', ' ') + "\","
    l += "\n};\n\n"

    if len(param_list) < 0x100:
        typ = "UINT8"
    elif len(param_list) < 0x10000:
        typ = "UINT16"
    else:
        raise

    s = s_b + typ + s_e
    stream.write(l + s)


def generate_cmd_param_range_lookup(hci_cmds_dom, stream=sys.stdout):
    """
    Generate a list of ranges for HCI LE Command parameters of type
    UINT8 or UINT16.
    """

    s_b = "/* Ordered listing of each command's UINT8/UINT16 parameter's range. */\n" +\
          "DECL_STATIC DECL_CONST "
    s_e = " cmd_prm_range[] =\n" +\
          "{"

    params_size_8 = 0
    params_size_16 = 0
    for cmd in hci_cmds_dom:
        if len(hci_cmds_dom[cmd]['CMD_PARAMS']) > 0:
            s_e += "\n   "
        for param in hci_cmds_dom[cmd]['CMD_PARAMS']:
            if hci_cmds_dom[cmd]['CMD_PARAMS'][param]['SIZE'] == 1:
                param_loc = 1
                params_size_8 += 1
            elif hci_cmds_dom[cmd]['CMD_PARAMS'][param]['SIZE'] == 2:
                param_loc = 1
                params_size_16 += 1
            else:
                param_loc = 0
            s_e += " " + str(param_loc) + ","

    s_e += "\n};\n"

    l = "" +\
        "" +\
        "\n" +\
        "typedef struct\n" +\
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
        "/* Parameter ranges list - UINT8. */\n" +\
        "DECL_STATIC DECL_CONST PARAM_RANGE_8 cmd_prm_range_list_8[] =\n" +\
        "{\n" +\
        "    {0, 0xFF},\n" +\
        "};\n" +\
        "\n" +\
        "/* Parameter ranges list - UINT16. */\n" +\
        "DECL_STATIC DECL_CONST PARAM_RANGE_16 cmd_prm_range_list_16[] =\n" +\
        "{\n" +\
        "    {0, 0xFFFF},\n" +\
        "};\n\n"

    greatest = (params_size_8, params_size_16)[params_size_16 > params_size_8]
    if greatest < 0x100:
        typ = "UINT8"
    elif greatest < 0x10000:
        typ = "UINT16"
    else:
        raise

    s = s_b + typ + s_e
    stream.write(l + s)


def generate_cmd_param_default_val_lookup(hci_cmds_dom, stream=sys.stdout):
    """
    Generate a list of deafult values for HCI LE Command parameters of type
    UINT8 or UINT16.
    """

    s_b = "/* Ordered listing of each command's UINT8/UINT16 parameter's default value. */\n" +\
          "DECL_STATIC DECL_CONST "
    s_e = " cmd_prm_dflt_val[] =\n" +\
          "{"

    params_size_8 = 0
    params_size_16 = 0
    params_array = 0
    for cmd in hci_cmds_dom:
        if len(hci_cmds_dom[cmd]['CMD_PARAMS']) > 0:
            s_e += "\n   "
        for param in hci_cmds_dom[cmd]['CMD_PARAMS']:
            if hci_cmds_dom[cmd]['CMD_PARAMS'][param]['SIZE'] == 1:
                param_loc = 1
                params_size_8 += 1
            elif hci_cmds_dom[cmd]['CMD_PARAMS'][param]['SIZE'] == 2:
                param_loc = 1
                params_size_16 += 1
            else:
                param_loc = 1
                params_array += 1
            s_e += " " + str(param_loc) + ","

    s_e += "\n};\n"

    l = "" +\
        "/* Parameter default values list - UINT8. */\n" +\
        "DECL_STATIC DECL_CONST UINT8 cmd_prm_def_val_list_8[] =\n" +\
        "{\n" +\
        "    0,\n" +\
        "};\n" +\
        "\n" +\
        "/* Parameter default values list - UINT16. */\n" +\
        "DECL_STATIC DECL_CONST UINT16 cmd_prm_def_val_list_16[] =\n" +\
        "{\n" +\
        "    0,\n" +\
        "};\n" +\
        "\n" +\
        "/* Parameter arrays default value. */\n" +\
        "DECL_STATIC DECL_CONST UINT8 cmd_prm_def_val_array[] =\n" +\
        "{\n" +\
        "    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, \n" +\
        "    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, \n" +\
        "    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, \n" +\
        "    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, \n" +\
        "};\n" +\
        "\n" +\
        "/* Parameter default values list - Arrays. */\n" +\
        "DECL_STATIC DECL_CONST UINT8* cmd_prm_def_val_list_arr[] =\n" +\
        "{\n" +\
        "    &cmd_prm_def_val_array[0],\n" +\
        "};\n\n"

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


def display_cmd_params_details_partial(hci_cmds_dom, stream=sys.stdout):
    """
    Prints some of the details (Name, Size, Bits) of HCI command parameters.
    Leaves room to manually type in other missing details such as default value
    and range.

    The generated file is in CSV format.
    """

    fields = OrderedDict([
        ('OCF', 6),
        ('HCI Command', 25),
        ('Param Name', 27),
        ('SIZE', 4),
        ('RANGE', 10),
        ('DEFAULT', 10),
        ('Valid Values', 15),
        ('Invalid Values', 15),
        ])

    s = ""

    for i, field in enumerate(fields):
        s += ("%*s| " % (fields[field], field))
        if i == 1:
            s = s[:-2]
            s += "\n"
    s = s[:-2]
    s += "\n"

    for cmd in hci_cmds_dom:
        s += "\n"
        s += ("0x%0*X| " % (fields['OCF']-2, hci_cmds_dom[cmd]['OCF']))
        s += ("%*s" % (fields['HCI Command'], cmd))
        s += "\n"
        for param in hci_cmds_dom[cmd]['CMD_PARAMS']:
            s += ("%*s| " % (fields['Param Name'], param))
            s += ("%*s| " % (fields['SIZE'], hci_cmds_dom[cmd]['CMD_PARAMS'][param]['SIZE']))
            s += ("%*s| " % (0, ""))
            s += ("%*s| " % (0, ""))
            s += ("%*s| " % (0, ""))
            s += ("%*s| " % (0, ""))
            s = s[:-2]
            s += "\n"

    stream.write(s)


if __name__ == "__main__":
    hci_parser = PdfHciParser("pdf_hci_cmds_parser_config.ini")

    files_list = [
        "./input/Core_v42_LE_Commands_Foxit.txt",                     # 0
        "./input/t_aberration_3_cmds_base.txt",                       # 1
        "./input/t_aberration_smmry_tbl_hdr.txt",                     # 2
        "./input/t_aberration_smmry_tbl_row.txt",                     # 3
        "./input/t_aberration_smmry_tbl_hdr_loc_parse_00.txt",        # 4
        "./input/t_aberration_smmry_tbl_hdr_loc_parse_01.txt",        # 5
        "./input/t_aberration_smmry_tbl_hdr_loc_parse_02.txt",        # 6
        "./input/t_aberration_smmry_tbl_hdr_loc_parse_03.txt",        # 7
        "./input/t_cmd_summary_tbl_Set_Event_Mask.txt",               # 8
        "./input/t_cmd_summary_tbl_Read_Buffer_Size.txt",             # 9
        "./input/t_cmd_summary_tbl_Set_Advertising_Params.txt",       # 10
        "./input/t_cmd_summary_tbl_Create_Connection.txt",            # 11
        "./input/t_desc_Set_Event_Mask.txt",                          # 12
        "./input/t_desc_Read_Buffer_Size.txt",                        # 13
        "./input/t_desc_Set_Advertising_Params.txt",                  # 14
        "./input/t_desc_Create_Connection.txt",                       # 15
        "./input/t_cmd_prm_size_Set_Event_Mask.txt",                  # 16
        "./input/t_cmd_prm_size_Read_Buffer_Size.txt",                # 17
        "./input/t_cmd_prm_size_Set_Advertising_Params.txt",          # 18
        "./input/t_cmd_prm_size_Create_Connection.txt",               # 19
        "./input/t_full_conn_update.txt",                             # 20
        "./input/t_full_read_buffer_size.txt",                        # 21
        "./input/t_full_read_spptd_states.txt",                       # 22
        "./input/t_full_set_adv_params.txt",                          # 23
        "./input/t_full_set_event_mask.txt",                          # 24
        "./input/t_full_start_encryption.txt",                        # 25
        "./input/t_full_generate_dh_key.txt",                         # 26
        "./input/t_cmd_param_name_mismatch_set_adv_params.txt",       # 27
        "./input/t_cmd_param_name_mismatch_set_event_mask.txt",       # 28
        "./input/t_cmd_param_name_mismatch_start_encryption.txt",     # 29
        "./input/t_ret_param_name_mismatch_set_adv_params.txt",       # 30
        "./input/t_ret_param_name_mismatch_set_event_mask.txt",       # 31
        "./input/t_ret_param_name_mismatch_read_buffer_size_01.txt",  # 32
        "./input/t_ret_param_name_mismatch_read_buffer_size_02.txt",  # 33
        ]
    f = open(files_list[0])

    hci_cmds = hci_parser.parse_pdf_text(f)

    if hci_cmds:
        f = open("parsed_hci_cmds.txt", "w")
        # generate_cmd_funcs(hci_cmds, f)
        # generate_cmd_param_lookup_tbl(hci_cmds, f)
        # generate_cmd_param_name_lookup(hci_cmds, f)
        # generate_cmd_param_range_lookup(hci_cmds, f)
        # generate_cmd_param_default_val_lookup(hci_cmds, f)
        # display_diff_param_len(hci_cmds)
        # display_cmd_params_len(hci_cmds, f)
        # display_ret_params_len(hci_cmds, f)
        # display_hci_cmds_details(hci_cmds, f)
        display_cmd_params_details_partial(hci_cmds, f)
