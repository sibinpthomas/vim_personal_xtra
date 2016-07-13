import os
from PIL import Image, ImageDraw

pu_filename = "thread_event_combinations_MSC.pu"
combo_png_name = "sequences_stitched.png"
seq_cnt = 0
thread_colours = ['Moccasin', 'Pink', 'LightBlue', 'LightGreen', 'LightCyan']


def stitch_seq_png_files():
    png_files_list = [fn for fn in os.listdir(".") if fn.endswith('.png')]

    images = map(Image.open, png_files_list)
    widths, heights = zip(*(i.size for i in images))

    total_height = sum(heights)
    max_width = max(widths)

    new_im = Image.new('RGB', (max_width, total_height))
    draw = ImageDraw.Draw(new_im)

    # Stitch files vertically.
    y_offset = 0
    for im in images:
        if y_offset != 0:
            draw.line((0, y_offset, im.size[0], y_offset),
                      fill="black", width=6)
        new_im.paste(im, (0, y_offset))
        y_offset += im.size[1]

    new_im.save(combo_png_name)


def gen_plantuml_msc(seq_num, seq, t1_evts, t2_evts, t1_name, t2_name):
    if seq_num <= 1:
        puf = open(pu_filename, "w")
    else:
        puf = open(pu_filename, "a")
        puf.write("\n")
        puf.write("\n")

    puf_header =\
        "@startuml 2_threads_sequence_" + ("%02d" % seq_num) + ".png\n" +\
        "scale 1.5\n" +\
        "title\n" +\
        "Sequence " + ("%d" % seq_num) + "\n" +\
        "end title\n" +\
        "hide footbox\n" +\
        "participant \"" + t1_name + "\"\n" +\
        "participant \"" + t2_name + "\"\n"

    puf_footer =\
        "|||\n" +\
        "@enduml"

    puf.write(puf_header)

    for evt in seq:
        if type(evt) is str:
            if evt in t1_evts:
                stmt = "rnote over \"" + t1_name + "\" #" + thread_colours[0] + " : " + evt
            else:
                stmt = "rnote over \"" + t2_name + "\" #" + thread_colours[1] + " : " + evt
        elif type(evt) is list:
            if evt in t1_evts:
                stmt = "rnote over \"" + t1_name + "\" #" + thread_colours[0] + "\n"
                for sub_evt in evt:
                    stmt += "* " + sub_evt + "\n"
                stmt += "endrnote"
            else:
                stmt = "rnote over \"" + t2_name + "\" #" + thread_colours[1] + "\n"
                for sub_evt in evt:
                    stmt += "* " + sub_evt + "\n"
                stmt += "endrnote"
        else:
            print "Event type is invalid - ",
            print evt
            exit()

        puf.write(stmt)
        puf.write("\n")

    puf.write(puf_footer)
    puf.close()


def recur(seq, t1_cvrd, t2_cvrd, t1_evts, t2_evts, t1_name, t2_name):
    """
    Recursive function that terminates upon finding a complete sequence with
    all thread events accounted for.
    """
    global seq_cnt
    is_t1_first = True

    if t1_cvrd == len(t1_evts):
        seq_cnt += 1
        final_seq = seq + t2_evts[t2_cvrd:]
        print str(seq_cnt) + ". ",
        print "".join([str(elem) for elem in final_seq])
        gen_plantuml_msc(seq_cnt, final_seq, t1_evts, t2_evts, t1_name, t2_name)
    elif t2_cvrd == len(t2_evts):
        seq_cnt += 1
        final_seq = seq + t1_evts[t1_cvrd:]
        print str(seq_cnt) + ". ",
        print "".join([str(elem) for elem in final_seq])
        gen_plantuml_msc(seq_cnt, final_seq, t1_evts, t2_evts, t1_name, t2_name)
    else:
        # print seq
        is_t1_first = (t1_cvrd <= t2_cvrd)

        if is_t1_first:
            recur(seq + [t1_evts[t1_cvrd]], t1_cvrd+1, t2_cvrd, t1_evts, t2_evts, t1_name, t2_name)
            recur(seq + [t2_evts[t2_cvrd]], t1_cvrd, t2_cvrd+1, t1_evts, t2_evts, t1_name, t2_name)
        else:
            recur(seq + [t2_evts[t2_cvrd]], t1_cvrd, t2_cvrd+1, t1_evts, t2_evts, t1_name, t2_name)
            recur(seq + [t1_evts[t1_cvrd]], t1_cvrd+1, t2_cvrd, t1_evts, t2_evts, t1_name, t2_name)


def gen_thread_event_combinations(t1_evts, t2_evts, t1_name, t2_name):
    print t1
    print t2

    recur([t1_evts[0]], 1, 0, t1_evts, t2_evts, t1_name, t2_name)
    recur([t2_evts[0]], 0, 1, t1_evts, t2_evts, t1_name, t2_name)

if __name__ == "__main__":
    # Events in Thread 1 in chronological order (earliest to latest)
    t1_name = "Thread 1"
    t1 = [["Lock M3",
           "Lock M1",
           "Set Curr Screen",
           "Unlock M3",
           ],
          "Wait on Stdin",
          "End of Wait on Stdin",
          ["Lock M2",
           "Reset Curr Screen",
           "Unlock M1",
           "Unlock M2"
           ]
          ]
    # Events in Thread 2 in chronological order (earliest to latest)
    t2_name = "Thread 2"
    t2 = ["Print Event",
          ["Lock M3",
           "Lock M2",
           "Reprint Curr Screen",
           "Unlock M2",
           "Unlock M3"
           ]
          ]

    # Delete existing .png and .pu files.
    for fn in os.listdir("."):
        if fn.endswith(".png") or fn.endswith(".pu"):
            os.remove(fn)

    # Generate PlantUML MSCs for each of the sequences the events in 2 threads
    # can occur in.
    gen_thread_event_combinations(t1, t2, t1_name, t2_name)

    # Process PlantUML file to generate a .png file for each event sequence.
    os.system("plantuml.jar " + pu_filename)

    # Stitich the generated .png files into a single .png file.
    stitch_seq_png_files()

    # Open the generated .png file.
    os.system(combo_png_name)
