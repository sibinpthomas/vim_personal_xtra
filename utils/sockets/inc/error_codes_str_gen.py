import logging
import re

logging.basicConfig(format='%(message)s',
                    level=logging.INFO + 1)

src_file = "sockets_export.h"

split_at = (".", "_")[1]
module_name = src_file.rsplit(split_at, 1)[0]
dest_file = module_name + "_error_codes_str.h"
logging.info(dest_file)

guard = "_H_" + (dest_file.rsplit(".", 1)[0]).upper() + "_"
logging.info(guard)

sf = open(src_file, "r")

err_strs = ""
err_dic = "#define " + module_name.upper() + "_ERROR_CODES \\\n"
for line in sf.readlines():
    mo = re.match("#define\s+([A-Z]\S+)\s+\(\(API_RESULT\)\(0[xX][0-9a-fA-F]{4}\s*\|\s*\S+\)\)", line)
    if mo:
        error_code = mo.group(1)
        oline = "#define " + error_code + "_STR"
        oline += " " * (43-len(oline)) + " "  # Column 45 or at least one space
        oline += "ERROR_STRING(" + error_code + ")"
        dline = "    {" + error_code + ", " + error_code + "_STR},\\"
        err_strs += oline + "\n"
        err_dic += dline + "\n"
sf.close()

logging.info(err_strs)
logging.info(err_dic)
f_str =\
    "#ifndef " + guard + "\n" +\
    "#define " + guard + "\n" +\
    "\n" +\
    "#include \"" + src_file + "\"" + "\n" +\
    "\n" +\
    "#ifdef __cplusplus" + "\n" +\
    "extern \"C\"" + "\n" +\
    "{" + "\n" +\
    "#endif /* __cplusplus */" + "\n" +\
    "\n" +\
    err_strs +\
    "\n" +\
    err_dic +\
    "\n" +\
    "#ifdef __cplusplus" + "\n" +\
    "}" + "\n" +\
    "#endif /* __cplusplus */" + "\n" +\
    "\n" +\
    "#endif /* " + guard + " */"

logging.info(f_str)
f = open(dest_file, "w")
f.write(f_str)
f.close()
