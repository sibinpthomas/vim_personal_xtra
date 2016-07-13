import inspect
import os.path


class logger(object):
    def __init__(self, log_to_stdout=True, filename="py_log.txt", append=False):

        self.logger_initialized = True
        self.logger_enabled = True
        self.log_to_stdout = log_to_stdout
        self.log_file_name = None

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

    def log_info(self, msg):
        """
        Prints the provided message in the format :

        [== INF ==]:FILENAME:LINENO: {message}
        """
        if self.logger_initialized is False or self.logger_enabled is False:
            return

        callerframerecord = inspect.stack()[1]
        frame = callerframerecord[0]
        info = inspect.getframeinfo(frame)

        log_str = "[== INF ==]:[%s]:[%d]: %s" % (os.path.basename(info.filename),
                                                 info.lineno, msg)
        if self.log_to_stdout is True:
            print(log_str)
        else:
            if self.log_file_name:
                f_hdl = open(self.log_file_name, "a")
                f_hdl.write(log_str)
                f_hdl.close()

    def log_error(self, msg):
        """
        Prints the provided message in the format :

        [** ERR **]:FILENAME:LINENO: {message}
        """
        if self.logger_initialized is False or self.logger_enabled is False:
            return

        callerframerecord = inspect.stack()[1]
        frame = callerframerecord[0]
        info = inspect.getframeinfo(frame)

        log_str = "[** ERR **]:[%s]:[%d]: %s" % (os.path.basename(info.filename),
                                                 info.lineno, msg)
        if self.log_to_stdout is True:
            print(log_str)
        else:
            if self.log_file_name:
                f_hdl = open(self.log_file_name, "a")
                f_hdl.write(log_str)
                f_hdl.close()

    def log_simple(self, msg):
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

    def log_str2hex(self, string):
        """
        Prints the ASCII hex value of each character in the provided string in the
        following format :

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
    lgr.log_info("Hello")
    lgr.disable_logger()
    lgr.log_simple("Ola!")
    lgr.enable_logger()
    lgr.log_simple("Ola!")
    lgr.log_error("World")
    lgr.log_str2hex("abcdefghijklmnop" * 3 + "123456ABCDEFGHIJK")
    lgr.shutdown_logger()
