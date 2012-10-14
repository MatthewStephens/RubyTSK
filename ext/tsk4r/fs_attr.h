//
//  fs_attr.h
//  RubyTSK
//
//  Created by ms3uf on 10/14/12.
//
//

#ifndef RubyTSK_fs_attr_h
#define RubyTSK_fs_attr_h

#include <tsk3/libtsk.h>
#include "tsk4r_i.h"

extern struct TSK_FS_ATTR fs_attr;

VALUE allocate_fs_attr(VALUE self);
void  deallocate_fs_attr(TSK_FS_ATTR * ptr);
VALUE initialize_fs_attr(int argc, VALUE *args, VALUE self);
VALUE fetch_attr(int argc, VALUE *args, VALUE self);

#endif
