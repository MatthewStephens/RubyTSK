//
//  fs_block.c
//  RubyTSK
//
//  Created by ms3uf on 10/13/12.
//
//

#include <stdio.h>
#include <ruby.h>
#include "fs_block.h"
#include "file_system.h"


VALUE allocate_fs_block(VALUE klass){
  TSK_FS_BLOCK * ptr;
  return Data_Make_Struct(klass, TSK_FS_BLOCK, 0, tsk_fs_block_free, ptr);
}


VALUE initialize_fs_block(int argc, VALUE *args, VALUE self) {
  VALUE filesystem; VALUE address;

  rb_scan_args(argc, args, "20", &filesystem, &address);
  rb_funcall(self, rb_intern("fetch"), 2, filesystem, address);

  return self;
}

VALUE fetch_block(VALUE self, VALUE filesystem, VALUE address) {
  TSK_FS_BLOCK * ptr; struct tsk4r_fs_wrapper * fs; TSK_DADDR_T addr;
  Data_Get_Struct(self, TSK_FS_BLOCK, ptr);
  Data_Get_Struct(filesystem, struct tsk4r_fs_wrapper, fs);
  addr = (TSK_DADDR_T)NUM2ULL(address);
  TSK_FS_INFO * fsystem;
  fsystem = fs->filesystem;

  ptr = tsk_fs_block_get(fsystem, NULL, addr);
  
  if (ptr != NULL) {
    rb_iv_set(self, "@address", ULONG2NUM(ptr->addr));
    rb_iv_set(self, "@buffer", rb_str_new2(ptr->buf));
    rb_iv_set(self, "@flags", UINT2NUM(ptr->flags));
    rb_iv_set(self, "@filesystem", filesystem);
    rb_iv_set(self, "@tag", INT2NUM(ptr->tag));
  } else {
    rb_warn("unable to get block");
  }
  return self;
}

