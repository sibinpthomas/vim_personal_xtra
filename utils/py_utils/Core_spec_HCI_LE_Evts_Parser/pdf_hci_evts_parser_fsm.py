import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
from logger import logger
from abstract_fsm import Abstract_FSM, Event_Handler, State
from abstract_fsm import Event, Abst_FSM_Excptn  # Imported for client modules.
from abstract_fsm import excptn as abstract_fsm_excptn
import pdf_hci_evts_parser_fsm_event_handlers as eh
from collections import OrderedDict


excptn = {}
excptn.update(abstract_fsm_excptn)


class pdf_hci_evts_parser_fsm(Abstract_FSM):
    """
    Pdf_hci_evts_parser_fsm class
    """

    def __init__(self, parser_config):
        self.pc = parser_config
        self.hci_evts = OrderedDict()
        self.evt_smmry_tbl = None

        self.lgr = logger()
        self.lgr.disable_logger()

        self.finito = False
        self.error_string = ""
        self.set_start_state("EVT_HEADING")

    def __str__(self):
        s = ""
        s += "\nHCI Event DOM -"
        for i, evt in enumerate(self.hci_evts):
            s += "\n" + str(i+1) + ". " + evt
            s += "\n\t Event Parameters -"
            for j, evt_param in enumerate(self.hci_evts[evt]['EVT_PARAMS']):
                s += "\n\t\t" + str(j+1) + ". " + evt_param
            s += "\n"
            s += "\n"

        return s

        s += "\n\n"
        s += "\n## Event Summary Table -"
        # s += "\n\t\"" + self.evt_smmry_tbl.hdr + "\""
        # s += "\n\t" + str(self.evt_smmry_tbl.evt_loc)
        # s += "\n\t" + str(self.evt_smmry_tbl.ocf_loc)
        # s += "\n\t" + str(self.evt_smmry_tbl.evt_params_loc)
        # s += "\n\t" + str(self.evt_smmry_tbl.ret_params_loc)
        # if self.evt_smmry_tbl.body:
        #     for l in self.evt_smmry_tbl.body:
        #         s += "\n\t>" + l
        s += "\n\t## " + self.evt_smmry_tbl.evt
        s += "\n\t## " + ("0x%04X" % self.evt_smmry_tbl.evt_code)
        # s += "\n" + self.desc
        s += "\n\t## " + str(self.evt_smmry_tbl.evt_params)
        for pmd in self.evt_smmry_tbl.evt_prms_meta_data:
            s += "\n\t\t* " + str(pmd)
        s += "\n\nAre we done yet? - " + ('No', 'Yes')[self.finito]
        if self.error_string:
            s += "\n!!!!!!!!! ERROR !!!!!!!!!!\n"
            s += self.error_string
            s += "\n!!!!!!!!!!!!!!!!!!!!!!!!!!"

        return s

    def __del__(self):
        pass

    __pdf_hci_evts_parser_fsm_fsm_desc = {
        "EVT_HEADING": State({"Ev_New_Line": Event_Handler(eh.is_evt_heading,
                                                           {'False': 'SELF_STATE',
                                                            'Error': 'ERROR',
                                                            'True': 'SUMMARY_TBL'}),
                              "Ev_EOF": Event_Handler(eh.gen_hci_evts_meta_data, "EXIT"),
                              }, None, None),
        "SUMMARY_TBL": State({"Ev_New_Line": Event_Handler(eh.is_smmry_tbl_hdr,
                                                           {'False': 'SELF_STATE',
                                                            'Error': 'ERROR',
                                                            'True': 'SUMMARY_TBL_SUB'}),
                              "Ev_EOF": Event_Handler(eh.gen_hci_evts_meta_data, "EXIT"),
                              }, None, None),
        "SUMMARY_TBL_SUB": State({"Ev_New_Line": Event_Handler(eh.is_tbl_row_desc_hdr,
                                                               {'False': 'SELF_STATE',
                                                                'Tbl_Row': 'SELF_STATE',
                                                                'Error': 'ERROR',
                                                                'Desc_Hdr': 'DESCRIPTION'}),
                                  "Ev_EOF": Event_Handler(eh.gen_hci_evts_meta_data, "EXIT"),
                                  }, None, eh.summary_tbl_sub_exit),
        "DESCRIPTION": State({"Ev_New_Line": Event_Handler(eh.is_desc_evt_prms_hdr,
                                                           {'False': 'SELF_STATE',
                                                            'Desc': 'SELF_STATE',
                                                            'Error': 'ERROR',
                                                            'Evt_Prms_Hdr': 'EVT_PARAMS'}),
                              "Ev_EOF": Event_Handler(eh.gen_hci_evts_meta_data, "EXIT"),
                              }, None, None),
        "EVT_PARAMS": State({"Ev_New_Line": Event_Handler(eh.is_evt_prm_hdr_evt_heading,
                                                          {'False': 'SELF_STATE',
                                                           'Error': 'ERROR',
                                                           'Subevent_Code': 'SUBEVENT_CODE_VAL',
                                                           'Event_Heading': 'SUMMARY_TBL',
                                                           'Evt_Prm_Hdr': 'EVT_PARAM_VALUE'}),
                             "Ev_EOF": Event_Handler(eh.gen_hci_evts_meta_data, "EXIT"),
                             }, None, None),
        "SUBEVENT_CODE_VAL": State({"Ev_New_Line": Event_Handler(eh.is_subevent_code,
                                                          {'False': 'SELF_STATE',
                                                           'Error': 'ERROR',
                                                           'True': 'EVT_PARAMS'}),
                             "Ev_EOF": Event_Handler(eh.gen_hci_evts_meta_data, "EXIT"),
                             }, None, None),
        "EVT_PARAM_VALUE": State({"Ev_New_Line": Event_Handler(eh.is_prm_val_evt_prm_hdr_evt_heading,
                                                               {'False': 'SELF_STATE',
                                                                'Prm_Val': 'SELF_STATE',
                                                                'Evt_Prm_Hdr': 'SELF_STATE',
                                                                'Error': 'ERROR',
                                                                'Event_Heading': 'SUMMARY_TBL'}),
                                  "Ev_EOF": Event_Handler(eh.gen_hci_evts_meta_data, "EXIT"),
                                  }, None, eh.evt_param_value_exit),
        "ERROR": State(None, eh.error_entry, None),
        "EXIT": State(None, eh.exit_entry, None),
        }

    @property
    def fsm_desc(self):
        return self.__pdf_hci_evts_parser_fsm_fsm_desc
