//
//  fs_file.c
//  RubyTSK
//
//  Created by ms3uf on 10/2/12.
//
//

#include <stdio.h>
#include <ruby.h>
#include "file_system.h"
#include "fs_file.h"

extern VALUE rb_cTSKFileSystem;
extern VALUE rb_cTSKFileSystemDir;
// internal function prototypes
void open_fs_file(VALUE self, VALUE source_obj, VALUE reference);
void open_fs_meta(VALUE self, VALUE source_obj, VALUE reference);
void open_fs_name(VALUE self, VALUE source_obj, VALUE reference);


// alloc & dealloc functions
VALUE allocate_fs_file(VALUE klass){
  struct tsk4r_fs_file_wrapper * ptr;
  return Data_Make_Struct(klass, struct tsk4r_fs_file_wrapper, 0, deallocate_fs_file, ptr);
}

VALUE allocate_fs_meta(VALUE klass){
  struct tsk4r_fs_meta_wrapper * ptr;
  return Data_Make_Struct(klass, struct tsk4r_fs_meta_wrapper, 0, deallocate_fs_meta, ptr);
}

VALUE allocate_fs_name(VALUE klass){
  struct tsk4r_fs_name_wrapper * ptr;
  return Data_Make_Struct(klass, struct tsk4r_fs_name_wrapper, 0, deallocate_fs_name, ptr);
}

void deallocate_fs_file(struct tsk4r_fs_file_wrapper * ptr){
  TSK_FS_FILE *file = ptr->file;
  tsk_fs_file_close(file);
  xfree(ptr);
}

void deallocate_fs_meta(struct tsk4r_fs_meta_wrapper * ptr){
  xfree(ptr);
}

void deallocate_fs_name(struct tsk4r_fs_name_wrapper * ptr){
  xfree(ptr);
}

// init
//VALUE initialize_fs_file(int argc, VALUE *args, VALUE self){
//  VALUE source_obj; VALUE opts; VALUE reference;
//  struct tsk4r_fs_dir_wrapper * fsdir_ptr;
//  Data_Get_Struct(self, struct tsk4r_fs_dir_wrapper, fsdir_ptr);
//  rb_scan_args(argc, args, "21", &source_obj, &reference, &opts);
//  
//  if ( RTEST(opts) != rb_cHash){
//    opts = rb_hash_new();
//  }
//  opts = rb_funcall(self, rb_intern("parse_opts"), 1, opts);
//  
//  open_fs_directory(self, source_obj, reference, opts);
//  if (fsdir_ptr->directory == NULL) {
//    rb_raise(rb_eRuntimeError, "could not access directory.");
//    return Qnil;
//  } else {
//    rb_iv_set(self, "@parent", source_obj);
//    return self;
//  }
//}
VALUE initialize_fs_file(int argc, VALUE *args, VALUE self){
  VALUE source_obj; VALUE reference; VALUE opts;
  rb_scan_args(argc, args, "21", &source_obj, &reference, &opts);
  open_fs_file(self, source_obj, reference);
  return self;
}

VALUE initialize_fs_meta(int argc, VALUE *args, VALUE self){
  VALUE source_obj; VALUE reference; VALUE opts;
  rb_scan_args(argc, args, "21", &source_obj, &reference, &opts);
  open_fs_meta(self, source_obj, reference);
  return self;
}

VALUE initialize_fs_name(int argc, VALUE *args, VALUE self){
  VALUE source_obj; VALUE reference; VALUE opts;
  rb_scan_args(argc, args, "21", &source_obj, &reference, &opts);
  open_fs_name(self, source_obj, reference);
  return self;
}


// private functions
void open_fs_file(VALUE self, VALUE fs, VALUE inum){
  struct tsk4r_fs_file_wrapper * fsfile;
  struct tsk4r_fs_wrapper * ptr;
  TSK_INUM_T addr;
  Data_Get_Struct(self, struct tsk4r_fs_file_wrapper, fsfile);
  Data_Get_Struct(fs, struct tsk4r_fs_wrapper, ptr);

  addr = inum;
  tsk_fs_file_open_meta(ptr->filesystem, fsfile->file, addr);
}
void open_fs_meta(VALUE self, VALUE fsfile, VALUE inum){
  struct tsk4r_fs_meta_wrapper * fsmeta;
  struct tsk4r_fs_file_wrapper * ptr;
  Data_Get_Struct(self, struct tsk4r_fs_meta_wrapper, fsmeta);
  Data_Get_Struct(fsfile, struct tsk4r_fs_file_wrapper, ptr);
  fsmeta->metadata = ptr->file->meta;

}
void open_fs_name(VALUE self, VALUE fsfile, VALUE inum){
  struct tsk4r_fs_name_wrapper * fsname;
  struct tsk4r_fs_file_wrapper * ptr;
  Data_Get_Struct(self, struct tsk4r_fs_name_wrapper, fsname);
  Data_Get_Struct(fsfile, struct tsk4r_fs_file_wrapper, ptr);
  
  fsname->name = ptr->file->name;
}
