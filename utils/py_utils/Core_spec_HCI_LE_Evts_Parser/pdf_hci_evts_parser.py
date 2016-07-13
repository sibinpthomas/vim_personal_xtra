import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
from logger import logger
from pdf_hci_evts_parser_fsm import pdf_hci_evts_parser_fsm, Event,\
    Abst_FSM_Excptn, excptn
import ConfigParser
import re

lgr = logger()


class PdfHciEvtsParserConfig(object):
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


class PdfHciEventParser(object):
    def __init__(self, config_file):
        self.pc = PdfHciEvtsParserConfig(config_file)

    def parse_pdf_text(self, f):
        # Instantiating FSM object
        pdf_hci_evt_parser_fsm_obj = pdf_hci_evts_parser_fsm(self.pc)

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
                    pdf_hci_evt_parser_fsm_obj.handle_event(event)
                    # lgr.log_info("##############################\n" + line)
                except Abst_FSM_Excptn as ae:
                    lgr.log_error(ae.value)
                    if ae.value[:len(excptn["Unexpected Event"])] !=\
                            excptn["Unexpected Event"]:
                        raise

                if pdf_hci_evt_parser_fsm_obj.finito:
                    break
        else:
            event = Event("Ev_EOF", None)

            try:
                pdf_hci_evt_parser_fsm_obj.handle_event(event)
            except Abst_FSM_Excptn as ae:
                lgr.log_error(ae.value)
                if ae.value[:len(excptn["Unexpected Event"])] !=\
                        excptn["Unexpected Event"]:
                    raise

        if pdf_hci_evt_parser_fsm_obj.error_string:
            print pdf_hci_evt_parser_fsm_obj.error_string
            return None
        else:
            # If there was no error during the parsing then return the full
            # HCI Events dictionary.
            return pdf_hci_evt_parser_fsm_obj.hci_evts


def display_hci_evts_details(hci_evts_dom, stream=sys.stdout):
    """
    HCI Events DOM Schema -
    -------------------------

    hci_evts_dom =
        {  # <Ordered Dictionary>

            HCI_EVT_01:
                {  # <Dictionary>

                    EVENT_CODE: <integer>
                    EVT_PARAMS:
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
                }

            HCI_EVT_02:
                {
                }
        }
    """

    s = ""
    s += "\nHCI Event DOM -"
    for i, evt in enumerate(hci_evts_dom):
        s += "\n" + str(i+1) + ". " + evt
        s += ("\n\t Subevent Code = 0x%02X" % hci_evts_dom[evt]['SUBEVENT_CODE'])
        s += "\n\t Event Parameters -"
        for j, evt_param in enumerate(hci_evts_dom[evt]['EVT_PARAMS']):
            s += "\n\t\t" + str(j+1) + ". " + evt_param
            s += "\n\t\t\t Size: %d" % hci_evts_dom[evt]['EVT_PARAMS'][evt_param]['SIZE']
            if hci_evts_dom[evt]['EVT_PARAMS'][evt_param]['BITS']:
                s += "\n\t\t\t Bits: %d" % hci_evts_dom[evt]['EVT_PARAMS'][evt_param]['BITS']
        s += "\n"
        s += "\n"

    stream.write(s)


def display_diff_param_len(hci_evts_dom, stream=sys.stdout):
    """
    Prints set of Event parameter sizes.
    """
    evt_params_len = set()
    for evt in hci_evts_dom:
        for evt_param in hci_evts_dom[evt]['EVT_PARAMS']:
            evt_params_len.add(hci_evts_dom[evt]['EVT_PARAMS'][evt_param]['SIZE'])

    print "The set of event parameter lengths - ",
    print sorted(evt_params_len)


def generate_evt_param_lookup_tbl(hci_evts_dom, stream=sys.stdout):
    """
    Prints lookup table to find data type of a event parameter.
    """
    tab = '    '

    types_list_dic = {1: 'UINT8_TYPE', 2: 'UINT16_TYPE',
                      6: 'ARR_UINT8_6_TYPE', 8: 'ARR_UINT8_8_TYPE',
                      32: 'ARR_UINT8_32_TYPE', 64: 'ARR_UINT8_64_TYPE'}

    types_list = "/* Set of the data types of the HCI LE event parameters.\n" +\
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
    meta_data_dump = "/* Ordered listing of each event's each parameter's meta-data. */\n"
    meta_data_dump += "DECL_STATIC DECL_CONST UINT8 evt_prm_meta_data[] =\n{"

    meta_data_loc  = "/* Start location of a HCI LE event's parameters meta-data in the above array.\n"
    meta_data_loc += " * '0' is used to indicate 'no event parameters'.\n"
    meta_data_loc += " * As a result the locations in this array are offset by '1'. */\n"
    meta_data_loc += "DECL_STATIC DECL_CONST UINT16 evt_prm_meta_data_loc[] =\n{"
    prev_evt = None
    evt_count = 0
    param_loc = 1
    param_loc_start = 0

    for evt in hci_evts_dom:
        if param_loc > param_loc_start:
            meta_data_dump += "\n" + tab[:-1]
        param_loc_start = param_loc
        for param in hci_evts_dom[evt]['EVT_PARAMS']:
            if hci_evts_dom[evt]['EVT_PARAMS'][param]['SIZE'] in types_list_dic:
                size = hci_evts_dom[evt]['EVT_PARAMS'][param]['SIZE']
                meta_data_dump += " " + types_list_dic[size] + ','
                param_loc += 1
            else:
                raise

        if prev_evt and hci_evts_dom[evt]['SUBEVENT_CODE'] > (hci_evts_dom[prev_evt]['SUBEVENT_CODE'] + 1):
            for ocf in xrange(hci_evts_dom[prev_evt]['SUBEVENT_CODE'] + 1, hci_evts_dom[evt]['SUBEVENT_CODE']):
                if (evt_count % break_num) == 0:
                    meta_data_loc += "\n" + tab
                meta_data_loc += str(0) + ", "
                evt_count += 1

        if evt_count % break_num == 0:
            meta_data_loc += "\n" + tab

        if param_loc > param_loc_start:
            meta_data_loc += str(param_loc_start) + ", "
        else:
            meta_data_loc += str(0) + ", "
        prev_evt = evt
        evt_count += 1

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


def generate_meta_evt_funcs(hci_evts_dom, stream=sys.stdout):
    """
    Generate HCI LE Meta Event functions which make use of the
    HCI LE common event API.
    """

    types_list_dic = {1: 'UINT8_TYPE', 2: 'UINT16_TYPE',
                      6: 'ARR_UINT8_6_TYPE', 8: 'ARR_UINT8_8_TYPE',
                      32: 'ARR_UINT8_32_TYPE', 64: 'ARR_UINT8_64_TYPE'}
    tab = '    '

    s = ""
    for evt in hci_evts_dom:
        func = "API_RESULT hci_" + evt.replace(' ', '_').lower() + "("
        s += func
        size_list = []
        for param in hci_evts_dom[evt]['EVT_PARAMS']:
            size = hci_evts_dom[evt]['EVT_PARAMS'][param]['SIZE']
            size_list.append(str(size))
            param = param.lower()
            if size in types_list_dic:
                typ = types_list_dic[size]
                if typ.startswith('ARR'):
                    s += "UINT8 (*" + param + ")[XXX_SIZE]" + ",\n"
                else:
                    s += typ.split('_')[0] + " " + param + ",\n"
                s += ' ' * len(func)
        else:
            s = s[:-(2 + len(func))]
        s += ")"
        s += "\n{"
        subevt_code_str = "0x%02Xu" % hci_evts_dom[evt]['SUBEVENT_CODE']
        s += "\n" + tab + "UINT8 subevt_code = " + subevt_code_str + ";"
        s += "\n" + tab + "HCI_LE_META_EVT_PARAMS hci_le_evt_meta_param;"
        s += "\n"
        s += "\n" + tab + "hci_le_evt_meta_param[0] = &subevt_code;"
        for i, param in enumerate(hci_evts_dom[evt]['EVT_PARAMS']):
            size = hci_evts_dom[evt]['EVT_PARAMS'][param]['SIZE']
            typ = types_list_dic[size]
            s += "\n" + tab + "hci_le_evt_meta_param[" + str(i+1) + "] = " +\
                 ('&', '')[typ.startswith('ARR')] + param.lower() + ";"
        else:
            s += "\n" + tab + "hci_le_evt_meta_param[" + str(i+2) + "] = NULL;"

        s += "\n"
        s += "\n" + tab + "return hci_le_meta_event_common_api(subevt_code, hci_le_evt_meta_param);"
        s += "\n}\n\n"

    stream.write(s)



if __name__ == "__main__":
    hci_event_parser = PdfHciEventParser("pdf_hci_evts_parser_config.ini")

    files_list = [
        "./input/Core_v42_LE_Events_Meta_Foxit.txt",                     # 0
        "./input/t_con_cmplt_full.txt",                                  # 1
        "./input/t_command_cmplt_full.txt",                              # 2
        ]
    f = open(files_list[0])

    hci_evts = hci_event_parser.parse_pdf_text(f)

    if hci_evts:
        f = open("parsed_hci_evts.txt", "w")
        # generate_evt_param_lookup_tbl(hci_evts, f)
        display_hci_evts_details(hci_evts, f)
        # generate_meta_evt_funcs(hci_evts, f)
        # display_diff_param_len(hci_evts)
        # display_hci_evts_details(hci_evts)
