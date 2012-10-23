//
//  fs_block.h
//  RubyTSK
//
//  Created by ms3uf on 10/13/12.
//
//

#ifndef RubyTSK_fs_block_h
#define RubyTSK_fs_block_h

#include <tsk3/libtsk.h>
#include "tsk4r_i.h"

extern struct TSK_FS_BLOCK fs_block;

VALUE allocate_fs_block(VALUE self);
VALUE initialize_fs_block(int argc, VALUE *args, VALUE self);
VALUE fetch_block(VALUE self, VALUE filesystem, VALUE address);

#endif
