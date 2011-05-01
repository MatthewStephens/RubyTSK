# tsk4r (The Sleuthkit for Ruby)
# version 0.0.1
# Copyright 2011 M. H. Stephens
require 'mkmf'

# permits passing of sleuthkit path as config parameter, so user can link to any tsk desired
# e.g. --with-sleuthkit-include=/path/to/tsk/include
# --with-sleuthkit-lib=/path/to/tsk/lib
# --with-sleuthkit-dir=/path/to/tsk is a shortcut for the previous two
dir_config("sleuthkit")

create_makefile("tsk4r")
