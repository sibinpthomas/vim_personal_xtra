"""
Abstract FSM

* Defines Abstract FSM class ('Abstract_FSM').
    This class will be the base class for all concrete FSM classes.
* Defines Abstract FSM Exception class ('Abst_FSM_Excptn').
* Defines dictionary of exceptions raised by the Abstract FSM.
* Defines the Immutability base class.
    This class is to be derived from to make the derived class immutable.
* Defines Event class.
* Defines Event Handler class ('Event_Handler').
* Defines State class.
"""

from types import FunctionType
from functools import wraps


#
# Dictionary which lists the exceptions raised by this module.
#
excptn = {
    "Invalid Input": "Invalid type for event parameter.",
    "In Unknown State": "The FSM is in an unknown state - ",
    "Invalid Next State": "Can't set the FSM to this unrecognized state - ",
    "Unexpected Event": "Event unexpected in current state - ",
    "Invalid Retval": "Couldn't find retval (of the event handler) in " +
                      "retval-next_state map - ",
    "State Entry Hdlr Fld": "State Entry handler failed.",
    "State Exit Hdlr Fld": "State Exit handler failed.",
    "Set State Fld": "Failed to set the state to - ",
    }


class Abst_FSM_Excptn(Exception):
    """
    Custom exception raised by the Abstract FSM class.

    Attributes:
        value - exception details in string format
    """
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)


class attr_ro_lock_meta(type):
    """
    Classes which need to make their attributes immutable should declare this
    class as its metaclass.

    This metaclass ensures that :
        * Class attributes can't be modified after they are set in the
            __init__() function.
        * No new attributes can be added (accidentally) to the object.
    """

    def __new__(meta, cname, bases, dctry):
        def _setattr(self, attr, value):
            if not hasattr(self, attr):
                # To prevent accidental creation of new class attributes (for
                # example, due to typos).
                raise AttributeError("'" + attr + "' not an attribute of " +
                                     cname + " object.")
            else:
                # Making class attributes write-protected.
                raise AttributeError("'" + attr + "' attribute of '" +
                                     cname + "' object is read-only.")

        def override_setattr_after(fn):
            @wraps(fn)
            def _wrapper(*args, **kwargs):
                cls.__setattr__ = object.__setattr__
                fn(*args, **kwargs)
                cls.__setattr__ = _setattr
            return _wrapper

        cls = type.__new__(meta, cname, bases, dctry)
        cls.__init__ = override_setattr_after(cls.__init__)
        return cls


class Event(object):
    """
    Generic Event object which all FSMs derived from the Abstract FSM must be
    capable of handling.

    Attributes:
        name - event name
        data - data associated with the event
    """

    __metaclass__ = attr_ro_lock_meta

    def __init__(self, name="", data=None):
        if not isinstance(name, basestring):
            raise TypeError("Event name has to be a string.")
        if len(name) == 0:
            raise ValueError("Event name can't be empty string.")

        self.name = name
        self.data = data

    def __str__(self):
        repr_str = "%s" % self.name
        if self.data:
            repr_str += " <%s>" % str(self.data)
        return repr_str

    def __repr__(self):
        repr_str = "Event -\n\tname: %s" % self.name
        if self.data:
            repr_str += "\n\tdata: %s" % str(self.data)
        else:
            repr_str += "\n\tdata: None"
        return repr_str


class Event_Handler(object):
    """
    Event Handler object.

    Attributes:
        eh_func    - Function to be invoked to handle the event (could be 'None').
        next_state - The next state the FSM is to be set to upon encountering
                     the event associated with this event handler.
                          - In a DFA this attribute is always a string.
                          - In a NFA this attribute could be either of
                              1. a string or
                              2. a dictionary which maps the return values of the
                                 event handler function to a valid next state.
    """

    __metaclass__ = attr_ro_lock_meta

    def __init__(self, eh_func=None, next_state=None):
        if eh_func is None or isinstance(eh_func, FunctionType):
            self.eh_func = eh_func
        else:
            raise TypeError("Given Event Handler Function is not of the right "
                            "type.")

        if isinstance(next_state, basestring) or isinstance(next_state, dict):
            if isinstance(next_state, dict) and eh_func is None:
                raise TypeError("When the event handler is 'Null' the next "
                                "state has to refer to a single state and not a"
                                " retval-next_state map.")
            elif isinstance(next_state, dict) and\
                    not all(isinstance(ns, basestring) for ns in next_state.values()):
                raise TypeError("A next state in the Retval-Next_State map "
                                "is not of the right type.")
            else:
                self.next_state = next_state
        else:
            raise TypeError("Given Next State is not of the right type.")

    def __str__(self):
        repr_str = ""
        if self.eh_func:
            repr_str = "%s" % (self.eh_func.__name__ + "()")
            if self.next_state:
                repr_str += " "
        if self.next_state:
            repr_str += "<%s>" % str(self.next_state)
        return repr_str

    def __repr__(self):
        repr_str = "Event Handler -\n\tfunction: "
        if self.eh_func:
            repr_str += (self.eh_func.__name__ + "()")
        else:
            repr_str += "NULL"
        if self.next_state:
            repr_str += "\n\tnext state: %s" % str(self.next_state)
        else:
            repr_str += "\n\tnext state: None"
        return repr_str


class State(object):
    """
    Object representing a FSM State.

    Attributes:
        eh_obj_dic - Dictionary of Event Handlers with their associated event as
                     the key.
        entry_func - State entry handler.
        exit_func  - State exit handler.
    """

    __metaclass__ = attr_ro_lock_meta

    def __init__(self, eh_obj_dic, entry_func=None, exit_func=None):
        if eh_obj_dic is None or (isinstance(eh_obj_dic, dict) and
                                  all(isinstance(eh, Event_Handler) for eh in eh_obj_dic.values())):
            self.eh_obj_dic = eh_obj_dic
        else:
            raise TypeError("Incorrect type, expected dictionary of "
                            "Event Handler objects.")

        if entry_func is None or isinstance(entry_func, FunctionType):
            self.entry_func = entry_func
        else:
            raise TypeError("Given State Entry handler is not of the right "
                            "type.")

        if exit_func is None or isinstance(exit_func, FunctionType):
            self.exit_func = exit_func
        else:
            raise TypeError("Given State Exit handler is not of the right "
                            "type.")


class fsm_cs_lock_meta(type):
    """
    Metaclass for the Abstract FSM class and all concrete FSM classes which
    derive from it.

    This metaclass ensures that :
        * '_curr_state' can't be modified.
        * No new attributes can be added (accidentally) to the object.
    """

    def __new__(meta, cname, bases, dctry):
        def _setattr_pre_init(self, attr, value):
            # '_curr_state' is write-protected.
            if attr is '_curr_state':
                raise AttributeError("'" + attr + "' can not be written to.")
            else:
                object.__setattr__(self, attr, value)

        def _setattr_post_init(self, attr, value):
            # To prevent accidental creation of new class attributes (for
            # example, due to typos).
            if not hasattr(self, attr):
                raise AttributeError("'" + attr + "' not an attribute of " +
                                     cname + " object.")
            else:
                _setattr_pre_init(self, attr, value)

        def override_setattr_after(fn):
            @wraps(fn)
            def _wrapper(*args, **kwargs):
                cls.__setattr__ = _setattr_pre_init
                fn(*args, **kwargs)
                cls.__setattr__ = _setattr_post_init
            return _wrapper

        cls = type.__new__(meta, cname, bases, dctry)
        cls.__init__ = override_setattr_after(cls.__init__)
        return cls


class Abstract_FSM(object):
    """
    Base class for all concrete FSM classes.
    Implements methods to handle an incoming event and to set the start state of
    the FSM.

    Defines the current state attribute and the setter and getter function for
    this attribute.
    """

    __metaclass__ = fsm_cs_lock_meta

    # Variable storing current state of the concrete FSM class.
    _curr_state = ""

    def __init__(self):
        pass

    def _get_state(self):
        return self._curr_state

    def _set_state(self, state):
        object.__setattr__(self, '_curr_state', state)
        return True

    def _is_valid_state(self, state):
        if (state in self.fsm_desc) or (state.upper() == 'SELF_STATE'):
            return True
        else:
            return False

    def _is_expected_event(self, curr_state, event):
        if curr_state in self.fsm_desc:
            if self.fsm_desc[curr_state].eh_obj_dic and\
                    event.name in self.fsm_desc[curr_state].eh_obj_dic:
                return True
            else:
                return False
        else:
            return False

    def set_start_state(self, start_state):
        """
        Sets the initial state of the FSM; typically called in the FSM
        constructor.
        """
        self.lgr.log_info("In set_start_state()")

        if not self._is_valid_state(start_state):
            raise Abst_FSM_Excptn(excptn["Invalid Next State"] + start_state)

        if not self._set_state(start_state):
            raise Abst_FSM_Excptn(excptn["Set State Fld"] + start_state)
        else:
            self.lgr.log_info("Initial State = " + start_state)

        # New State entry handler
        entry_func = self.fsm_desc[start_state].entry_func
        if entry_func:
            if not entry_func(self):
                raise Abst_FSM_Excptn(excptn["State Entry Hdlr Fld"])

    def handle_event(self, event):
        """
        Handles incoming event and modifies the instance of the FSM the method
        is called on.
        """
        if not isinstance(event, Event):
            raise Abst_FSM_Excptn(excptn["Invalid Input"])

        self.lgr.log_simple("\n-----------------------------------------------")
        self.lgr.log_info("Received Event = " + event.name)
        curr_state = self._get_state()
        self.lgr.log_info("Current State = " + curr_state)

        if not self._is_valid_state(curr_state):
            raise Abst_FSM_Excptn(excptn["In Unknown State"] + curr_state)

        if not self._is_expected_event(curr_state, event):
            raise Abst_FSM_Excptn(excptn["Unexpected Event"] + event.name)

        evt_obj_dic = self.fsm_desc[curr_state].eh_obj_dic

        # Event handler
        evt_handler = evt_obj_dic[event.name].eh_func
        if evt_handler:
            retval = evt_handler(self, event.data)

        # Setting next state
        if isinstance(evt_obj_dic[event.name].next_state, dict):
            if retval in evt_obj_dic[event.name].next_state:
                next_state = evt_obj_dic[event.name].next_state[retval]
                self.lgr.log_info("The event handler returned - '" + retval + "'")
            else:
                raise Abst_FSM_Excptn(excptn["Invalid Retval"] + str(retval))
        else:  # Assumed to be string
            next_state = evt_obj_dic[event.name].next_state

        if not self._is_valid_state(next_state):
            raise Abst_FSM_Excptn(excptn["Invalid Next State"] + next_state)

        if next_state != curr_state and next_state.upper() != 'SELF_STATE':
            # Current State exit handler
            exit_func = self.fsm_desc[curr_state].exit_func
            if exit_func:
                if not exit_func(self):
                    raise Abst_FSM_Excptn(excptn["State Exit Hdlr Fld"])

            # Setting next state
            if not self._set_state(next_state):
                raise Abst_FSM_Excptn(excptn["Set State Fld"] + next_state)
            else:
                self.lgr.log_info("New State = " + next_state)

            # New State entry handler
            entry_func = self.fsm_desc[next_state].entry_func
            if entry_func:
                if not entry_func(self):
                    raise Abst_FSM_Excptn(excptn["State Entry Hdlr Fld"])
