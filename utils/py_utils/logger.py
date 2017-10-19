import inspect
import os.path


class logger(object):
    def __init__(self, log_to_stdout=True, logger_level=0, filename="py_log.txt", append=False):

        self.logger_initialized = True
        self.logger_enabled = True
        self.log_to_stdout = log_to_stdout
        self.log_file_name = None
        self.logger_level = logger_level

        if log_to_stdout is False:
            self.log_file_name = filename
            if append is False:
                # Erasing contents of file.
                f_hdl = open(self.log_file_name, "w")
                f_hdl.close()

    def shutdown_logger(self):

        if not self.logger_initialized:
            return

        # Set to initial state
        self.logger_initialized = False
        self.logger_enabled = False
        self.log_to_stdout = True
        self.log_file_name = None

    def enable_logger(self):
        self.logger_enabled = True

    def disable_logger(self):
        self.logger_enabled = False

    def set_logger_level(self, lvl):
        if lvl >= 0 and lvl <= 6:
            self.logger_level = lvl

    def log_simple(self, msg):
        if self.logger_level <= 1:
            self._log_simple(msg)

    def log_str2hex(self, string):
        if self.logger_level <= 1:
            self._log_str2hex(string)

    def log_trace(self, msg, module_name=""):
        """
        Prints the provided message in the format :

        [-- TRC --]:FILENAME:LINENO:[module_name] {message}
        """
        if self.logger_level <= 1:
            msg_prefix = "-- TRC --"
            self._log_formatted(msg, msg_prefix, module_name)

    def log_info(self, msg, module_name=""):
        """
        Prints the provided message in the format :

        [== INF ==]:FILENAME:LINENO:[module_name] {message}
        """
        if self.logger_level <= 2:
            msg_prefix = "== INF =="
            self._log_formatted(msg, msg_prefix, module_name)

    def log_warning(self, msg, module_name=""):
        """
        Prints the provided message in the format :

        [~~ WARN ~~]:FILENAME:LINENO:[module_name] {message}
        """
        if self.logger_level <= 3:
            msg_prefix = "~~ WARN ~~"
            self._log_formatted(msg, msg_prefix, module_name)

    def log_error(self, msg, module_name=""):
        """
        Prints the provided message in the format :

        [** ERR **]:FILENAME:LINENO:[module_name] {message}
        """
        if self.logger_level <= 4:
            msg_prefix = "** ERR **"
            self._log_formatted(msg, msg_prefix, module_name)

    def log_critical(self, msg, module_name=""):
        """
        Prints the provided message in the format :

        [!! CTCL !!]:FILENAME:LINENO:[module_name] {message}
        """
        if self.logger_level <= 5:
            msg_prefix = "!! CTCL !!"
            self._log_formatted(msg, msg_prefix, module_name)

    def log_temp(self, msg, module_name=""):
        """
        Prints the provided message in the format :

        [!! TEMP !!]:FILENAME:LINENO:[module_name] {message}
        """
        if self.logger_level <= 6:
            msg_prefix = "!! TEMP !!"
            self._log_formatted(msg, msg_prefix, module_name)

    def log_simple_temp(self, msg):
        self._log_simple(msg)

    def log_simple_str2hex(self, string):
        self._log_str2hex(string)

    def _log_formatted(self, msg, msg_prefix, module_name):
        """
        Prints the provided message in the format :

        [msg_prefix]:FILENAME:LINENO:[module_name] {message}
                            or (if module_name not provided)
        [msg_prefix]:FILENAME:LINENO: {message}
        """
        if self.logger_initialized is False or self.logger_enabled is False:
            return

        if module_name:
            # The expectation is that the 'module_name' parameter is only
            # provided by module specific log functions which are a thin
            # abstraction over the logger's log functions, hence we go 1 level
            # deeper in the stack to obtain the actual level at which the log
            # function was invoked in the code under observation.
            module_name = '[' + module_name + ']'
            callerframerecord = inspect.stack()[3]
        else:
            callerframerecord = inspect.stack()[2]
        frame = callerframerecord[0]
        info = inspect.getframeinfo(frame)

        log_str = "[%s]:[%s]:[%d]:%s %s" % (msg_prefix,
                                            os.path.basename(info.filename),
                                            info.lineno,
                                            module_name,
                                            msg)
        if self.log_to_stdout is True:
            print(log_str)
        else:
            if self.log_file_name:
                f_hdl = open(self.log_file_name, "a")
                f_hdl.write(log_str)
                f_hdl.close()

    def _log_simple(self, msg):
        """
        Prints the provided message.
        """
        if self.logger_initialized is False or self.logger_enabled is False:
            return

        if self.log_to_stdout is True:
            print(msg)
        else:
            if self.log_file_name:
                f_hdl = open(self.log_file_name, "a")
                f_hdl.write(msg)
                f_hdl.close()

    def _log_str2hex(self, string):
        """
        Prints the ASCII hex value of each character in the provided string in
        the following format :

        [** HEX **]:FILENAME:LINENO:
            beg-end: {str2hex}
        """
        if self.logger_initialized is False or self.logger_enabled is False:
            return

        callerframerecord = inspect.stack()[1]
        frame = callerframerecord[0]
        info = inspect.getframeinfo(frame)
        format = "%02X"
        hex_per_line = 16
        width_per_char = len(format % 12) + 1  # 1 is for separator(e.g. space)

        hex_list = [("%02X" % ord(c)) for c in string]
        hex_str = " ".join(hex_list)
        hex_table = ""
        curr_char = 0
        hex_beg = curr_char * width_per_char
        hex_end = (curr_char+hex_per_line) * width_per_char
        hex_line = hex_str[hex_beg:hex_end]

        while len(hex_line) == (hex_per_line*width_per_char):
            line_span = ("%10s" % ("%d-%d: " % (curr_char, curr_char+16-1)))
            hex_table += line_span + hex_line + "\n"
            curr_char += 16
            hex_beg = curr_char * width_per_char
            hex_end = (curr_char+hex_per_line) * width_per_char
            hex_line = hex_str[hex_beg:hex_end]
        else:
            num_chr = ((len(hex_line)+1)/width_per_char)
            line_span = ("%10s" % ("%d-%d: " % (curr_char, curr_char+num_chr-1)))
            hex_table += line_span + hex_line + "\n"

        log_str = "[xx HEX xx]:[%s]:[%d]: \n%s" % (os.path.basename(info.filename),
                                                   info.lineno, hex_table)

        if self.log_to_stdout is True:
            print(log_str),
        else:
            if self.log_file_name:
                f_hdl = open(self.log_file_name, "a")
                f_hdl.write(log_str)
                f_hdl.close()


if __name__ == "__main__":
    lgr = logger()

    module_name = ['', 'LGR', 'TEST', 'MAIN', '', 'CBUF', 'SOCK']
    for logger_lvl in xrange(7):
        lgr.set_logger_level(logger_lvl)
        print "\n------ Iteration at Logger level - %d ------\n" % logger_lvl
        lgr.log_simple("SIMPLE\n")
        lgr.log_str2hex("abcdefghijklmnop" * 3 + "123456ABCDEFGHIJK")
        lgr.log_trace("TRACE\n")
        lgr.log_info("INFO\n", module_name[logger_lvl])
        lgr.log_warning("WARNING\n", module_name[logger_lvl])
        lgr.log_error("ERROR\n")
        lgr.log_critical("CRITICAL\n")
        lgr.log_temp("TEMPORARY\n")
        lgr.log_simple_temp("SIMPLE TEMPORARY\n")
        lgr.log_simple_str2hex("abcdefghijklmnop" * 3 + "123456ABCDEFGHIJK")

    lgr.shutdown_logger()
