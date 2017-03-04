import sys, os

base_path = os.path.dirname(os.path.abspath(__file__))

import ycmflags

def FlagsForFile( filename ):
    flags = ycmflags.YcmFlags(default_file = ["gui/mainwindow.cpp", []],
            absolute_project_path = base_path)
    return flags.flags_for_file(filename)
