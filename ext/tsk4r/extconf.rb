# encoding: UTF-8
# tsk4r (tsk4r) (Ruby bindings for the SleuthKit C library)
# version 0.0.1
# Copyright 2011 ___Matthew H. Stephens___
require 'mkmf'

# permits passing of a library's path as config parameter, so user can link to any version desired at build time
# e.g. --with-somelib-include=/path/to/somelib/include
# --with-somelib-lib=/path/to/somelib/lib
# --with-somelib-dir=/path/to/somelib is a shortcut for the previous two

HEADER_DIRS=[
"/usr/local/include",
#"/usr/local/include/tsk3"
]
LIB_DIRS=[
"/usr/local/lib"
]

# # allow user configuration
dir_config("tsk3", HEADER_DIRS, LIB_DIRS)
# 
# # require presense of third-party library
have_library("tsk3")
# 
# # don't build makefile without this header from which to link somelib to your code
 unless find_header('tsk3/libtsk.h') # note: some libraries are 'name/libname.h', most are simply 'name.h'
   abort "tsk3/libtsk.h is missing.  please install tsk3/libtsk.h"
 end

# 1.9 compatibility
$CFLAGS += " -DRUBY_19" if RUBY_VERSION =~ /^1\.9/

# override normal build configuration to build debug friendly library
# if installed via 'gem install sleuthkit -- --enable-debug'

if enable_config('debug')
	puts '[INFO] enabling debug library build configuration.'
	if RUBY_VERSION < '1.9'
		$CFLAGS = CONFIG['CFLAGS'].gsub(/\s\-O\d?\s/, ' -O0 ')
		$CFLAGS.gsub!(/\s?\-g\w*\s/, ' -ggdb3 ')
		CONFIG['LDSHARED'] = CONFIG['LDSHARED'].gsub(/\s\-s(\s|\z)/, ' ')
	else
		CONFIG['debugflags'] << ' -ggdb3 -O0'
	end
end

# if you want to change compilers
#Config::MAKEFILE_CONFIG["CC"]='/usr/bin/gcc'
#puts "Creating makefile with Config::MAKEFILE_CONFIG['CC'] = #{Config::MAKEFILE_CONFIG['CC']}"

create_makefile("tsk4r/tsk4r")
