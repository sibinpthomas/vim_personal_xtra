import re
from collections import namedtuple
from collections import OrderedDict


class PdfHciCmdsParserError(Exception):
    """
    Custom exception raised by Parsers.

    Attributes:
        msg - exception details in string format
    """
    def __init__(self, msg):
        self.msg = msg

    def __str__(self):
        return repr(self.msg)


class cmd_summary_tbl(object):
    def __init__(self):
        self.body = []
        self.cmd_loc = -1
        self.ocf_loc = -1
        self.cmd_params_loc = -1
        self.ret_params_loc = -1
        self.cmd = ""
        self.ocf = ""
        self.cmd_params_str = ""
        self.ret_params_str = ""
        self.cmd_params = []
        self.ret_params = []
        self.cmd_prms_meta_data = []
        self.ret_prms_meta_data = []
        self.desc = ""
        self.gen_events = ""


pattern_retval = namedtuple('pattern_retval', ['regex', 'action', 'retval'])
param_meta_data = namedtuple('param_meta_data', ['size', 'bits', 'default', 'range'])
param_range = namedtuple('param_range', ['min', 'max'])


#
# State Entry\Exit handlers.
#
def summary_tbl_sub_exit(inst):
    inst.lgr.log_info("In summary_tbl_sub_exit().")

    for l in inst.cmd_smmry_tbl.body:
        length = len(l)
        done = False

        start = 0
        end = 0
        loc_list = [inst.cmd_smmry_tbl.ocf_loc,
                    inst.cmd_smmry_tbl.cmd_params_loc,
                    inst.cmd_smmry_tbl.ret_params_loc,
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
                    raise PdfHciCmdsParserError("For loop didn't break.")

            # Trim white-space and hyphens off from the edges.
            # Replace any space in between the text segment with an underscore.
            text_seg = (l[start:end].strip(inst.pc.WS+inst.pc.hyphen)).replace(" ", "_")
            text_seg = text_seg.replace("__", "_")
            if i == 0:
                inst.cmd_smmry_tbl.cmd += text_seg
            elif i == 1:
                inst.cmd_smmry_tbl.ocf += text_seg
            elif i == 2:
                inst.cmd_smmry_tbl.cmd_params_str += text_seg
            elif i == 3:
                inst.cmd_smmry_tbl.ret_params_str += text_seg
            if done:
                break

    if not inst.cmd_smmry_tbl.cmd.startswith('HCI_'):
        inst.cmd_smmry_tbl.cmd = "HCI_" + inst.cmd_smmry_tbl.cmd
    inst.cmd_smmry_tbl.ocf = int(inst.cmd_smmry_tbl.ocf, 0)
    inst.cmd_smmry_tbl.cmd_params = inst.cmd_smmry_tbl.cmd_params_str.split(",")
    inst.cmd_smmry_tbl.ret_params = inst.cmd_smmry_tbl.ret_params_str.split(",")
    return 'True'


def cmd_param_value_exit(inst):
    inst.lgr.log_info("In cmd_param_value_exit().")
    return 'True'


def ret_param_value_exit(inst):
    inst.lgr.log_info("In ret_param_value_exit().")
    return 'True'


def gen_events_exit(inst):
    inst.lgr.log_info("In gen_events_exit().")

    cst = inst.cmd_smmry_tbl
    inst.hci_cmds[cst.cmd] = {}
    inst.hci_cmds[cst.cmd]['OCF'] = cst.ocf

    inst.hci_cmds[cst.cmd]['CMD_PARAMS'] = OrderedDict()
    for cmd_param_name, meta_data in zip(cst.cmd_params, cst.cmd_prms_meta_data):
        inst.hci_cmds[cst.cmd]['CMD_PARAMS'][cmd_param_name] = OrderedDict()
        cmd_param = inst.hci_cmds[cst.cmd]['CMD_PARAMS'][cmd_param_name]
        cmd_param['SIZE'] = meta_data.size
        cmd_param['BITS'] = meta_data.bits
        cmd_param['RANGE'] = meta_data.range
        cmd_param['DEFAULT'] = meta_data.default

    inst.hci_cmds[cst.cmd]['RET_PARAMS'] = OrderedDict()
    for ret_param_name, meta_data in zip(cst.ret_params, cst.ret_prms_meta_data):
        inst.hci_cmds[cst.cmd]['RET_PARAMS'][ret_param_name] = OrderedDict()
        ret_param = inst.hci_cmds[cst.cmd]['RET_PARAMS'][ret_param_name]
        ret_param['SIZE'] = meta_data.size
        ret_param['BITS'] = meta_data.bits
        ret_param['RANGE'] = meta_data.range
        ret_param['DEFAULT'] = meta_data.default

    inst.hci_cmds[cst.cmd]['DESCRIPTION'] = cst.desc
    inst.hci_cmds[cst.cmd]['GEN_EVENTS'] = cst.gen_events

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
def is_cmd_heading(inst, data):
    inst.lgr.log_info("In is_cmd_heading().")

    # Expected Patterns.
    exp_patterns = [pattern_retval(inst.pc.cmd_hdr_line, None, 'True')]
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
        inst.cmd_smmry_tbl = cmd_summary_tbl()
        if len(m_obj.groups()) == 4:
            inst.cmd_smmry_tbl.cmd_loc = m_obj.start(1)
            inst.cmd_smmry_tbl.ocf_loc = m_obj.start(2)
            inst.cmd_smmry_tbl.cmd_params_loc = m_obj.start(3)
            inst.cmd_smmry_tbl.ret_params_loc = m_obj.start(4)
        else:
            raise PdfHciCmdsParserError("The Command Summary table header line is wonky," +
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
            inst.error_string = "Found aberrant pattern while seeking Command "\
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
        inst.cmd_smmry_tbl.body.append(line)

    exp_patterns = [pattern_retval(inst.pc.desc_hdr_line, None, 'Desc_Hdr'),
                    pattern_retval(inst.pc.smmry_tbl_row, tbl_row_action, 'Tbl_Row')]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking Command "\
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


def is_desc_cmd_prms_hdr(inst, data):
    inst.lgr.log_info("In is_desc_cmd_prms_hdr().")

    def tbl_row_action(inst, line, line_num, m_obj):
        inst.cmd_smmry_tbl.desc += line

    exp_patterns = [pattern_retval(inst.pc.cmd_prms_hdr_line, None, 'Cmd_Prms_Hdr'),
                    pattern_retval(inst.pc.desc_row, tbl_row_action, 'Desc')]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking "\
                                "command Description details.\n"\
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


def _decode_cmd_prm_hdr(inst, line, line_num, m_obj):
    cmd_prm = (m_obj.group(1)).replace(" ", "_")
    cmd_param_list_l = [cmd_param.lower() for cmd_param in inst.cmd_smmry_tbl.cmd_params]
    if cmd_prm.lower() in cmd_param_list_l:
        bits = None
        if m_obj.group(4):
            bits = int(m_obj.group(4))
        p_md = param_meta_data(int(m_obj.group(2)), bits, None, None)
        inst.cmd_smmry_tbl.cmd_prms_meta_data.append(p_md)
    else:
        raise PdfHciCmdsParserError("The Command Parameter *" +
                                    m_obj.group(1) + "* was not found amongst the " +
                                    "parameters mentioned in the Command Summary Table" +
                                    " (" + ", ".join(inst.cmd_smmry_tbl.cmd_params) +
                                    "), at line number - " +
                                    str(line_num))


def is_cmd_prm_hdr_ret_prms_hdr(inst, data):
    inst.lgr.log_info("In is_cmd_prm_hdr_ret_prms_hdr().")

    exp_patterns = [pattern_retval(inst.pc.ret_prms_hdr_line, None, 'Ret_Prms_Hdr'),
                    pattern_retval(inst.pc.cmd_prm_size_line, _decode_cmd_prm_hdr, 'Cmd_Prm_Hdr')]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking Command "\
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


def is_prm_val_cmd_prm_hdr_ret_prms_hdr(inst, data):
    inst.lgr.log_info("In is_prm_val_cmd_prm_hdr_ret_prms_hdr().")

    exp_patterns = [pattern_retval(inst.pc.ret_prms_hdr_line, None, 'Ret_Prms_Hdr'),
                    pattern_retval(inst.pc.cmd_prm_size_line, _decode_cmd_prm_hdr, 'Cmd_Prm_Hdr'),
                    pattern_retval(inst.pc.cmd_prm_val, None, 'Prm_Val'),
                    ]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking Command "\
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


def _decode_ret_prm_hdr(inst, line, line_num, m_obj):
    ret_prm = (m_obj.group(1)).replace(" ", "_")
    ret_param_list_l = [ret_param.lower() for ret_param in inst.cmd_smmry_tbl.ret_params]
    if ret_prm.lower() in ret_param_list_l:
        bits = None
        if m_obj.group(4):
            bits = int(m_obj.group(4))
        p_md = param_meta_data(int(m_obj.group(2)), bits, None, None)
        inst.cmd_smmry_tbl.ret_prms_meta_data.append(p_md)
    else:
        raise PdfHciCmdsParserError("The Return Parameter *" +
                                    m_obj.group(1) + "* was not found amongst the " +
                                    "parameters mentioned in the Command Summary Table" +
                                    " (" + ", ".join(inst.cmd_smmry_tbl.ret_params) +
                                    "), at line number - " +
                                    str(line_num))


def is_ret_prm_hdr_gen_evts(inst, data):
    inst.lgr.log_info("In is_ret_prm_hdr_gen_evts().")

    exp_patterns = [pattern_retval(inst.pc.evts_gen_hdr_line, None, 'Gen_Evts'),
                    pattern_retval(inst.pc.ret_prm_size_line, _decode_ret_prm_hdr, 'Ret_Prm_Hdr')
                    ]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking "\
                                "Return Parameters header.\n"\
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


def is_prm_val_ret_prm_hdr_gen_evts(inst, data):
    inst.lgr.log_info("In is_prm_val_ret_prm_hdr_gen_evts().")

    exp_patterns = [pattern_retval(inst.pc.evts_gen_hdr_line, None, 'Gen_Evts'),
                    pattern_retval(inst.pc.ret_prm_size_line, _decode_ret_prm_hdr, 'Ret_Prm_Hdr'),
                    pattern_retval(inst.pc.ret_prm_val, None, 'Prm_Val'),
                    ]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking Return "\
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


def is_gen_evts_cmd_hdr(inst, data):
    inst.lgr.log_info("In is_gen_evts_cmd_hdr().")

    def evts_gen_dets_action(inst, line, line_num, m_obj):
        inst.cmd_smmry_tbl.gen_events += line

    exp_patterns = [pattern_retval(inst.pc.cmd_hdr_line, None, 'Cmd_Hdr'),
                    pattern_retval(inst.pc.evts_gen_details, evts_gen_dets_action, 'Gen_Evts')]

    # Patterns which are definitiely errors in this state and not to be silently
    # ignored.
    aberrant_patterns = inst.pc.section_beg_patterns - {e.regex for e in exp_patterns}
    for aber_pat in aberrant_patterns:
        if re.match(aber_pat, data[0]):
            inst.error_string = "Found aberrant pattern while seeking "\
                                "generated events details.\n"\
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


def gen_hci_cmds_meta_data(inst, data):
    inst.lgr.log_info("In gen_hci_cmds_meta_data().")

    return 'True'
