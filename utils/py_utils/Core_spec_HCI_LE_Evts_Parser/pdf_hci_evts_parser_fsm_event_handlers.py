import re
from collections import namedtuple
from collections import OrderedDict


class PdfHciEvtsParserError(Exception):
    """
    Custom exception raised by Parsers.

    Attributes:
        msg - exception details in string format
    """
    def __init__(self, msg):
        self.msg = msg

    def __str__(self):
        return repr(self.msg)


class evt_summary_tbl(object):
    def __init__(self):
        self.body = []
        self.evt_loc = -1
        self.evt_code_loc = -1
        self.evt_params_loc = -1
        self.evt = ""
        self.evt_code = ""
        self.subevt_code = ""
        self.evt_params_str = ""
        self.evt_params = []
        self.evt_prms_meta_data = []
        self.desc = ""


pattern_retval = namedtuple('pattern_retval', ['regex', 'action', 'retval'])
param_meta_data = namedtuple('param_meta_data', ['size', 'bits', 'default', 'range'])
param_range = namedtuple('param_range', ['min', 'max'])


#
# State Entry\Exit handlers.
#
def summary_tbl_sub_exit(inst):
    inst.lgr.log_info("In summary_tbl_sub_exit().")

    for l in inst.evt_smmry_tbl.body:
        length = len(l)
        done = False

        start = 0
        end = 0
        loc_list = [inst.evt_smmry_tbl.evt_code_loc,
                    inst.evt_smmry_tbl.evt_params_loc,
                    99999999  # Ridiculously large value for length of PDF line.
                    ]
        for i, loc in enumerate(loc_list):
            start = end
            if length < loc:
                end = length
                done = True
            else:
                for end in xrange(loc, loc-(inst.pc.ws_margin+1), -1):
                    if l[end] == ' ':
                        break
                else:
                    raise PdfHciEvtsParserError("For loop didn't break.")

            # Trim white-space and hyphens off from the edges.
            # Replace any space in between the text segment with an underscore.
            text_seg = l[start:end].strip(inst.pc.WS)
            if i == 0:
                if text_seg and text_seg[-1:] != '-':
                    text_seg += ' '
                else:
                    text_seg = text_seg[:-1]
                inst.evt_smmry_tbl.evt += text_seg
            elif i == 1:
                inst.evt_smmry_tbl.evt_code += text_seg
            elif i == 2:
                inst.evt_smmry_tbl.evt_params_str += text_seg
            if done:
                break

    inst.evt_smmry_tbl.evt = inst.evt_smmry_tbl.evt.rstrip(inst.pc.WS).replace(" ", "_")
    inst.evt_smmry_tbl.evt_code = int(inst.evt_smmry_tbl.evt_code, 0)
    inst.evt_smmry_tbl.evt_params = inst.evt_smmry_tbl.evt_params_str.split(",")[1:]
    return 'True'


def evt_param_value_exit(inst):
    inst.lgr.log_info("In evt_param_value_exit().")

    est = inst.evt_smmry_tbl
    inst.hci_evts[est.evt] = {}
    inst.hci_evts[est.evt]['EVENT_CODE'] = est.evt_code
    inst.hci_evts[est.evt]['SUBEVENT_CODE'] = est.subevt_code

    inst.hci_evts[est.evt]['EVT_PARAMS'] = OrderedDict()
    for evt_param_name, meta_data in zip(est.evt_params, est.evt_prms_meta_data):
        inst.hci_evts[est.evt]['EVT_PARAMS'][evt_param_name] = OrderedDict()
        evt_param = inst.hci_evts[est.evt]['EVT_PARAMS'][evt_param_name]
        evt_param['SIZE'] = meta_data.size
        evt_param['BITS'] = meta_data.bits
        evt_param['RANGE'] = meta_data.range
        evt_param['DEFAULT'] = meta_data.default

    inst.hci_evts[est.evt]['DESCRIPTION'] = est.desc

    return 'True'


def error_entry(inst):
    inst.lgr.log_info("In error_entry().")

    inst.finito = True
    return 'True'


def exit_entry(inst):
    inst.lgr.log_info("In exit_entry().")

    inst.finito = True
    return 'True'


#
# Event Handlers.
#
def is_evt_heading(inst, data):
    inst.lgr.log_info("In is_evt_heading().")

    # Expected Patterns.
    exp_patterns = [pattern_retval(inst.pc.evt_hdr_line, None, 'True')]
    for exp_pat in exp_patterns:
        m_obj = re.match(exp_pat.regex, data[0])
        if m_obj:
            if exp_pat.action:
                exp_pat.action(inst, data[0], data[1], m_obj)
            return exp_pat.retval

    return 'False'


def is_smmry_tbl_hdr(inst, data):
    inst.lgr.log_info("In is_smmry_tbl_hdr().")

    def smmry_tbl_hdr_line_action(inst, line, line_num, m_obj):
        inst.evt_smmry_tbl = evt_summary_tbl()
        if len(m_obj.groups()) == 3:
            inst.evt_smmry_tbl.evt_loc = m_obj.start(1)
            inst.evt_smmry_tbl.evt_code_loc = m_obj.start(2)
            inst.evt_smmry_tbl.evt_params_loc = m_obj.start(3)
        else:
            raise PdfHciEvtsParserError("The Event Summary table header line is wonky," +
                                        " at line number - " +
                                        str(line_num))

    exp_patterns = [pattern_retval(inst.pc.smmry_tbl_hdr_line,
                                   smmry_tbl_hdr_line_action,
                                   'True')]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking Event "\
                                "Summary Table header.\n"\
                                "Aberrant Pattern - \"" + data[0] + "\""
            return 'Error'

    # Checking for match with any of the Expected Patterns.
    for exp_pat in exp_patterns:
        m_obj = re.match(exp_pat.regex, data[0])
        if m_obj:
            if exp_pat.action:
                exp_pat.action(inst, data[0], data[1], m_obj)
            return exp_pat.retval

    return 'False'


def is_tbl_row_desc_hdr(inst, data):
    inst.lgr.log_info("In is_tbl_row_desc_hdr().")

    def tbl_row_action(inst, line, line_num, m_obj):
        inst.evt_smmry_tbl.body.append(line)

    exp_patterns = [pattern_retval(inst.pc.desc_hdr_line, None, 'Desc_Hdr'),
                    pattern_retval(inst.pc.smmry_tbl_row, tbl_row_action, 'Tbl_Row')]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking Event "\
                                "Summary table row.\n"\
                                "Aberrant Pattern - \"" + data[0] + "\""
            return 'Error'

    # Checking for match with any of the Expected Patterns.
    for exp_pat in exp_patterns:
        m_obj = re.match(exp_pat.regex, data[0])
        if m_obj:
            if exp_pat.action:
                exp_pat.action(inst, data[0], data[1], m_obj)
            return exp_pat.retval

    return 'False'


def is_desc_evt_prms_hdr(inst, data):
    inst.lgr.log_info("In is_desc_evt_prms_hdr().")

    def tbl_row_action(inst, line, line_num, m_obj):
        inst.evt_smmry_tbl.desc += line

    exp_patterns = [pattern_retval(inst.pc.evt_prms_hdr_line, None, 'Evt_Prms_Hdr'),
                    pattern_retval(inst.pc.desc_row, tbl_row_action, 'Desc')]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking "\
                                "event Description details.\n"\
                                "Aberrant Pattern - \"" + data[0] + "\""
            return 'Error'

    # Checking for match with any of the Expected Patterns.
    for exp_pat in exp_patterns:
        m_obj = re.match(exp_pat.regex, data[0])
        if m_obj:
            if exp_pat.action:
                exp_pat.action(inst, data[0], data[1], m_obj)
            return exp_pat.retval

    return 'False'


def _decode_evt_prm_hdr(inst, line, line_num, m_obj):
    evt_prm = (m_obj.group(1)).replace(" ", "_")
    event_param_list_l = [evt_param.lower() for evt_param in inst.evt_smmry_tbl.evt_params]
    if evt_prm.lower() in event_param_list_l:
        bits = None
        if m_obj.group(4):
            bits = int(m_obj.group(4))
        p_md = param_meta_data(int(m_obj.group(2)), bits, None, None)
        inst.evt_smmry_tbl.evt_prms_meta_data.append(p_md)
    else:
        raise PdfHciEvtsParserError("The Event Parameter *" +
                                    evt_prm + "* was not found amongst the " +
                                    "parameters mentioned in the Event Summary Table" +
                                    " (" + ", ".join(inst.evt_smmry_tbl.evt_params) +
                                    "), at line number - " +
                                    str(line_num))


def is_evt_prm_hdr_evt_heading(inst, data):
    inst.lgr.log_info("In is_evt_prm_hdr_evt_heading().")

    exp_patterns = [pattern_retval(inst.pc.evt_hdr_line, None, 'Event_Heading'),
                    pattern_retval(inst.pc.subevent_code_size_line, None, 'Subevent_Code'),
                    pattern_retval(inst.pc.evt_prm_size_line, _decode_evt_prm_hdr, 'Evt_Prm_Hdr')]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking Event "\
                                "Parameter header wth size.\n"\
                                "Aberrant Pattern - \"" + data[0] + "\""
            return 'Error'

    # Checking for match with any of the Expected Patterns.
    for exp_pat in exp_patterns:
        m_obj = re.match(exp_pat.regex, data[0])
        if m_obj:
            if exp_pat.action:
                exp_pat.action(inst, data[0], data[1], m_obj)
            return exp_pat.retval

    return 'False'


def is_subevent_code(inst, data):
    inst.lgr.log_info("In is_smmry_tbl_hdr().")

    def decode_subevent_code(inst, line, line_num, m_obj):
        inst.evt_smmry_tbl.subevt_code = int(m_obj.group(1), 0)

    exp_patterns = [pattern_retval(inst.pc.subevent_code_val_line,
                                   decode_subevent_code,
                                   'True')]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking "\
                                "Subevent Code value.\n"\
                                "Aberrant Pattern - \"" + data[0] + "\""
            return 'Error'

    # Checking for match with any of the Expected Patterns.
    for exp_pat in exp_patterns:
        m_obj = re.match(exp_pat.regex, data[0])
        if m_obj:
            if exp_pat.action:
                exp_pat.action(inst, data[0], data[1], m_obj)
            return exp_pat.retval

    return 'False'


def is_prm_val_evt_prm_hdr_evt_heading(inst, data):
    inst.lgr.log_info("In is_prm_val_evt_prm_hdr_evt_heading().")

    exp_patterns = [pattern_retval(inst.pc.evt_hdr_line, None, 'Event_Heading'),
                    pattern_retval(inst.pc.evt_prm_size_line, _decode_evt_prm_hdr, 'Evt_Prm_Hdr'),
                    pattern_retval(inst.pc.evt_prm_val, None, 'Prm_Val'),
                    ]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking Event "\
                                "Parameter value details.\n"\
                                "Aberrant Pattern - \"" + data[0] + "\""
            return 'Error'

    # Checking for match with any of the Expected Patterns.
    for exp_pat in exp_patterns:
        m_obj = re.match(exp_pat.regex, data[0])
        if m_obj:
            if exp_pat.action:
                exp_pat.action(inst, data[0], data[1], m_obj)
            return exp_pat.retval

    return 'False'


def gen_hci_evts_meta_data(inst, data):
    inst.lgr.log_info("In gen_hci_evts_meta_data().")

    return 'True'
