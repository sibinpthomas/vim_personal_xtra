import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
from logger import logger
from abstract_fsm import Abstract_FSM, Event_Handler, State
from abstract_fsm import Event, Abst_FSM_Excptn  # Imported for client modules.
from abstract_fsm import excptn as abstract_fsm_excptn
import pdf_hci_cmds_parser_fsm_event_handlers as eh
from collections import OrderedDict


excptn = {}
excptn.update(abstract_fsm_excptn)


class pdf_hci_cmds_parser_fsm(Abstract_FSM):
    """
    Pdf_hci_parser_fsm class
    """

    def __init__(self, parser_config):
        self.pc = parser_config
        self.hci_cmds = OrderedDict()
        self.cmd_smmry_tbl = None

        self.lgr = logger()
        self.lgr.disable_logger()

        self.finito = False
        self.error_string = ""
        self.set_start_state("CMD_HEADING")

    def __str__(self):
        s = ""
        s += "\nHCI Command DOM -"
        for i, cmd in enumerate(self.hci_cmds):
            s += "\n" + str(i+1) + ". " + cmd
            s += "\n\t Command Parameters -"
            for j, cmd_param in enumerate(self.hci_cmds[cmd]['CMD_PARAMS']):
                s += "\n\t\t" + str(j+1) + ". " + cmd_param
            s += "\n"
            s += "\n\t Return Parameters -"
            for j, ret_param in enumerate(self.hci_cmds[cmd]['RET_PARAMS']):
                s += "\n\t\t" + str(j+1) + ". " + ret_param
            s += "\n"
            s += "\n"

        return s

        s += "\n\n"
        s += "\n## Command Summary Table -"
        # s += "\n\t\"" + self.cmd_smmry_tbl.hdr + "\""
        # s += "\n\t" + str(self.cmd_smmry_tbl.cmd_loc)
        # s += "\n\t" + str(self.cmd_smmry_tbl.ocf_loc)
        # s += "\n\t" + str(self.cmd_smmry_tbl.cmd_params_loc)
        # s += "\n\t" + str(self.cmd_smmry_tbl.ret_params_loc)
        # if self.cmd_smmry_tbl.body:
        #     for l in self.cmd_smmry_tbl.body:
        #         s += "\n\t>" + l
        s += "\n\t## " + self.cmd_smmry_tbl.cmd
        s += "\n\t## " + ("0x%04X" % self.cmd_smmry_tbl.ocf)
        # s += "\n" + self.desc
        s += "\n\t## " + str(self.cmd_smmry_tbl.cmd_params)
        for pmd in self.cmd_smmry_tbl.cmd_prms_meta_data:
            s += "\n\t\t* " + str(pmd)
        s += "\n\t## " + str(self.cmd_smmry_tbl.ret_params)
        for pmd in self.cmd_smmry_tbl.ret_prms_meta_data:
            s += "\n\t\t* " + str(pmd)
        s += "\n\nAre we done yet? - " + ('No', 'Yes')[self.finito]
        if self.error_string:
            s += "\n!!!!!!!!! ERROR !!!!!!!!!!\n"
            s += self.error_string
            s += "\n!!!!!!!!!!!!!!!!!!!!!!!!!!"

        return s

    def __del__(self):
        pass

    __pdf_hci_parser_fsm_fsm_desc = {
        "CMD_HEADING": State({"Ev_New_Line":
                              Event_Handler(eh.is_cmd_heading,
                                            {'False': 'SELF_STATE',
                                             'Error': 'ERROR',
                                             'True': 'SUMMARY_TBL'}),
                              "Ev_EOF":
                              Event_Handler(eh.gen_hci_cmds_meta_data, "EXIT"),
                              }, None, None),
        "SUMMARY_TBL": State({"Ev_New_Line":
                              Event_Handler(eh.is_smmry_tbl_hdr,
                                            {'False': 'SELF_STATE',
                                             'Error': 'ERROR',
                                             'True': 'SUMMARY_TBL_SUB'}),
                              "Ev_EOF":
                              Event_Handler(eh.gen_hci_cmds_meta_data, "EXIT"),
                              }, None, None),
        "SUMMARY_TBL_SUB": State({"Ev_New_Line":
                                  Event_Handler(eh.is_tbl_row_desc_hdr,
                                                {'False': 'SELF_STATE',
                                                 'Tbl_Row': 'SELF_STATE',
                                                 'Error': 'ERROR',
                                                 'Desc_Hdr': 'DESCRIPTION'}),
                                  "Ev_EOF":
                                  Event_Handler(eh.gen_hci_cmds_meta_data, "EXIT"),
                                  }, None, eh.summary_tbl_sub_exit),
        "DESCRIPTION": State({"Ev_New_Line":
                              Event_Handler(eh.is_desc_cmd_prms_hdr,
                                            {'False': 'SELF_STATE',
                                             'Desc': 'SELF_STATE',
                                             'Error': 'ERROR',
                                             'Cmd_Prms_Hdr': 'CMD_PARAMS'}),
                              "Ev_EOF":
                              Event_Handler(eh.gen_hci_cmds_meta_data, "EXIT"),
                              }, None, None),
        "CMD_PARAMS": State({"Ev_New_Line":
                             Event_Handler(eh.is_cmd_prm_hdr_ret_prms_hdr,
                                           {'False': 'SELF_STATE',
                                            'Error': 'ERROR',
                                            'Ret_Prms_Hdr': 'RET_PARAMS',
                                            'Cmd_Prm_Hdr': 'CMD_PARAM_VALUE'}),
                             "Ev_EOF":
                             Event_Handler(eh.gen_hci_cmds_meta_data, "EXIT"),
                             }, None, None),
        "CMD_PARAM_VALUE": State({"Ev_New_Line":
                                  Event_Handler(eh.is_prm_val_cmd_prm_hdr_ret_prms_hdr,
                                                {'False': 'SELF_STATE',
                                                 'Prm_Val': 'SELF_STATE',
                                                 'Cmd_Prm_Hdr': 'SELF_STATE',
                                                 'Error': 'ERROR',
                                                 'Ret_Prms_Hdr': 'RET_PARAMS'}),
                                  "Ev_EOF":
                                  Event_Handler(eh.gen_hci_cmds_meta_data, "EXIT"),
                                  }, None, eh.cmd_param_value_exit),
        "RET_PARAMS": State({"Ev_New_Line":
                             Event_Handler(eh.is_ret_prm_hdr_gen_evts,
                                           {'False': 'SELF_STATE',
                                            'Error': 'ERROR',
                                            'Gen_Evts': 'GEN_EVENTS',
                                            'Ret_Prm_Hdr': 'RET_PARAM_VALUE'}),
                             "Ev_EOF":
                             Event_Handler(eh.gen_hci_cmds_meta_data, "EXIT"),
                             }, None, None),
        "RET_PARAM_VALUE": State({"Ev_New_Line":
                                  Event_Handler(eh.is_prm_val_ret_prm_hdr_gen_evts,
                                                {'False': 'SELF_STATE',
                                                 'Prm_Val': 'SELF_STATE',
                                                 'Ret_Prm_Hdr': 'SELF_STATE',
                                                 'Error': 'ERROR',
                                                 'Gen_Evts': 'GEN_EVENTS'}),
                                  "Ev_EOF":
                                  Event_Handler(eh.gen_hci_cmds_meta_data, "EXIT"),
                                  }, None, eh.ret_param_value_exit),
        "GEN_EVENTS": State({"Ev_New_Line":
                             Event_Handler(eh.is_gen_evts_cmd_hdr,
                                           {'False': 'SELF_STATE',
                                            'Gen_Evts': 'SELF_STATE',
                                            'Error': 'ERROR',
                                            'Cmd_Hdr': 'SUMMARY_TBL'}),
                             "Ev_EOF":
                             Event_Handler(eh.gen_hci_cmds_meta_data, "EXIT"),
                             }, None, eh.gen_events_exit),
        "ERROR": State(None, eh.error_entry, None),
        "EXIT": State(None, eh.exit_entry, None),
        }

    @property
    def fsm_desc(self):
        return self.__pdf_hci_parser_fsm_fsm_desc
