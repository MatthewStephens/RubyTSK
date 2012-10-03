//
//  fs_dir.c
//  RubyTSK
//
//  Created by Matthew Stephens on 10/1/12.
//
//

#include <stdio.h>
#include <ruby.h>
#include "file_system.h"
#include "fs_dir.h"


extern VALUE rb_cTSKImage;
extern VALUE rb_cTSKVolumeSystem;
extern VALUE rb_cTSKVolumePart;
extern VALUE rb_cTSKFileSystem;
extern VALUE rb_cTSKFileSystemDir;

// creation functions
VALUE allocate_fs_dir(VALUE klass){
  struct tsk4r_fs_dir_wrapper * ptr;
  return Data_Make_Struct(klass, struct tsk4r_fs_dir_wrapper, 0, deallocate_fs_dir, ptr);
}

void deallocate_fs_dir(struct tsk4r_fs_dir_wrapper * ptr){
  TSK_FS_DIR *directory = ptr->directory;
  tsk_fs_dir_close(directory);
  xfree(ptr);
}

VALUE initialize_fs_dir(int argc, VALUE *args, VALUE self){
  VALUE source_obj; VALUE opts; VALUE reference;
  struct tsk4r_fs_dir_wrapper * fsdir_ptr;
  Data_Get_Struct(self, struct tsk4r_fs_dir_wrapper, fsdir_ptr);
  rb_scan_args(argc, args, "21", &source_obj, &reference, &opts);
  
  if ( RTEST(opts) != rb_cHash){
    opts = rb_hash_new();
  }
  opts = rb_funcall(self, rb_intern("parse_opts"), 1, opts);

  open_fs_directory(self, source_obj, reference, opts);
  if (fsdir_ptr->directory == NULL) {
    rb_raise(rb_eRuntimeError, "could not access directory.");
    return Qnil;
  } else {
    rb_iv_set(self, "@parent", source_obj);
    return self;
  }
}

VALUE open_fs_directory(VALUE self, VALUE parent_obj, VALUE name_or_inum, VALUE opts) {
  struct tsk4r_fs_dir_wrapper * fsdir_ptr; struct tsk4r_fs_wrapper * fs_ptr;
  Data_Get_Struct(self, struct tsk4r_fs_dir_wrapper, fsdir_ptr);
  Data_Get_Struct(parent_obj, struct tsk4r_fs_wrapper, fs_ptr);
  
  if ( rb_obj_is_instance_of(name_or_inum, rb_cFixnum) ) {
    printf("opening by inum: %lu\n", FIX2LONG(name_or_inum));
    TSK_INUM_T addr = (TSK_INUM_T)FIX2LONG(name_or_inum);
    fsdir_ptr->directory = tsk_fs_dir_open_meta(fs_ptr->filesystem, addr);
    if (fsdir_ptr->directory == NULL) { printf("opened dir, returning to init.\n"); }
  }
  else if ( rb_obj_is_instance_of(name_or_inum, rb_cString) ) {
    printf("opening by name: %s\n", StringValuePtr(name_or_inum));
    const char *dir_pathname = StringValuePtr(name_or_inum);
    fsdir_ptr->directory = tsk_fs_dir_open(fs_ptr->filesystem, dir_pathname);
    // populate an iv while we have both structs
  } else {
    rb_raise(rb_eTypeError, "Arg2 must be a String or Fixnum.");
  }
  return self;
}