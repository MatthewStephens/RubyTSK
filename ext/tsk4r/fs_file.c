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

VALUE initialize_fs_file(int argc, VALUE *args, VALUE self){
  VALUE source_obj; VALUE reference; VALUE opts;
  rb_scan_args(argc, args, "21", &source_obj, &reference, &opts);
  rb_iv_set(self, "@parent", source_obj);
  printf("init here, using rb_funcall to open_fs_file\n");
  rb_funcall(self, rb_intern("open_fs_file"), 1, reference);
//  open_fs_file(self, reference);
  return self;
}

VALUE initialize_fs_meta(int argc, VALUE *args, VALUE self){
  VALUE source_obj; VALUE reference; VALUE opts;
  rb_scan_args(argc, args, "21", &source_obj, &reference, &opts);
  rb_iv_set(self, "@parent", source_obj);

  open_fs_meta(self, source_obj, reference);
  return self;
}

VALUE initialize_fs_name(int argc, VALUE *args, VALUE self){
  VALUE source_obj; VALUE reference; VALUE opts;
  rb_scan_args(argc, args, "21", &source_obj, &reference, &opts);
  rb_iv_set(self, "@parent", source_obj);
  
  open_fs_name(self, source_obj, reference);
  return self;
}


// private functions
VALUE open_fs_file(int argc, VALUE *args, VALUE self){
  VALUE inum;
  printf("open_fs_file here\n");
  rb_scan_args(argc, args, "10", &inum);
  printf("called with %d arguments\n", argc);

  struct tsk4r_fs_file_wrapper * fsfile;
  struct tsk4r_fs_wrapper * ptr;
  const TSK_TCHAR *addr_string;
  VALUE parent;
  parent = rb_iv_get(self, "@parent");
  Data_Get_Struct(self, struct tsk4r_fs_file_wrapper, fsfile);
  Data_Get_Struct(parent, struct tsk4r_fs_wrapper, ptr);
  TSK_FS_ATTR_TYPE_ENUM type = TSK_FS_ATTR_TYPE_DEFAULT;
  uint8_t type_used = 0; // image type? (autodetect)
  uint16_t id = 0;
  uint8_t id_used = 0;
  TSK_INUM_T parsed_inum;

  //tsk offers an inum parsing function, so convert FIXNUM to C string and use it
  VALUE inum_to_str = rb_funcall(inum, rb_intern("to_s"), 0, NULL);
  addr_string = StringValuePtr(inum_to_str);
  tsk_fs_parse_inum(addr_string, &parsed_inum, &type, &type_used, &id, &id_used);

  TSK_FS_INFO * system  = ptr->filesystem;
  TSK_FS_FILE * tskfile = fsfile->file;

  tsk_fs_file_open_meta(system, tskfile, (TSK_INUM_T)parsed_inum);
//
  if (tskfile != NULL) {
    printf("recieved metadata for inum %llu\n", parsed_inum);
//    rb_iv_set(self, "@content_len", LONG2FIX(tskfile->meta->content_len));
//    rb_iv_set(self, "@address", INT2FIX(tskfile->meta->addr));
  } else {
    rb_warn("unable to perform tsk_fs_file_open_meta");
  }
  return self;

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
