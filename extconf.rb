# tsk4r (The Sleuthkit for Ruby)
# version 0.0.1
# Copyright 2011 M. H. Stephens
require 'mkmf'

# permits passing of sleuthkit path as config parameter, so user can link to any tsk desired
# e.g. --with-sleuthkit-include=/path/to/tsk/include
# --with-sleuthkit-lib=/path/to/tsk/lib
# --with-sleuthkit-dir=/path/to/tsk is a shortcut for the previous two
HEADER_DIRS=[
"/Users/ms3uf/RubyTSK/sleuthkit-3.2.1",
"/usr/local/include"
]
LIB_DIRS=[
"/usr/local/lib"
]
dir_config("tsk3", HEADER_DIRS, LIB_DIRS)
have_library("tsk3")
unless find_header('tsk3/libtsk.h')
  abort "libtsk.h is missing.  please install libtsk.h"
end
create_makefile("tsk4r")

