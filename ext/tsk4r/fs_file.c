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
#include "fs_dir.h"

extern VALUE rb_cTSKFileSystem;
extern VALUE rb_cTSKFileSystemDir;
extern VALUE rb_cTSKFileSystemFileData;
extern VALUE rb_cTSKFileSystemFileMeta;
extern VALUE rb_cTSKFileSystemFileName;

extern void klassify();
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

// init can be called in several ways
// 1. creation of FileSystem::Directory calls it to build
// file object to represent the directory  #new(filesystem, dir, opts)
// 2. an inum or name can be used to seek a file on its own #new(filesystem, name_or_inum, opts)

VALUE initialize_fs_file(int argc, VALUE *args, VALUE self) {
  VALUE fs; VALUE reference; VALUE opts; TSK_INUM_T addr;
  rb_scan_args(argc, args, "21", &fs, &reference, &opts);
  printf("initialize_fs_file here...\n");
  klassify(fs, "fs");
  klassify(reference, "reference");
  rb_iv_set(self, "@parent", fs);
  
  // get filesystem, self
  struct tsk4r_fs_file_wrapper * fs_file;
  struct tsk4r_fs_wrapper * fs_ptr;
  TSK_FS_INFO * filesystem;

  Data_Get_Struct(fs, struct tsk4r_fs_wrapper, fs_ptr);
  Data_Get_Struct(self, struct tsk4r_fs_file_wrapper, fs_file);
  filesystem = fs_ptr->filesystem;
  
  // determine if reference is a directory obj, file name, or metadata entry (e.g. inum)
  if (rb_obj_is_kind_of(reference, rb_cTSKFileSystemDir)) {
    printf("building FILE object from Dir\n");
    struct tsk4r_fs_dir_wrapper * fs_dir;
    Data_Get_Struct(reference, struct tsk4r_fs_dir_wrapper, fs_dir);
    addr = fs_dir->directory->addr;

    printf("NOT calling tsk_fs_file_open_meta(%llu) (from a directory object)\n", addr);
    // if directory already built, we should have TSK_FS_FILE pointer
    fs_file->file = fs_dir->directory->fs_file;
    
  } else if (rb_obj_is_kind_of(reference, rb_cFixnum)) {
    printf("building FILE object from Fixnum\n");

    addr = (TSK_INUM_T)FIX2ULONG(reference);

    tsk_fs_file_open_meta(filesystem, fs_file->file, addr);
    
  } else if (rb_obj_is_kind_of(reference, rb_cString)) {
    printf("building FILE object from String\n");

    TSK_TCHAR * name;
    name = StringValuePtr(reference);
    printf("calling tsk_fs_file_open(%s)\n", name);
    tsk_fs_file_open(filesystem, fs_file->file, name);
    
  } else {
    rb_warn("arg2 should be a FileSystem::Directory object, Fixnum, or String!");
  }

  if (fs_file->file != NULL) {
    printf("Successful access to TSK_FS_FILE\n");
    rb_iv_set(self, "@meta", rb_funcall(rb_cTSKFileSystemFileMeta, rb_intern("new"), 1, self));
  } else {
    rb_warn("fs_file->file is NULL!");
  }
  
  return self;
}

// #new(file, opts) should only be called from within fs_file's #init
// open up the API as follows:
// #new(rb_cTSKFileSystem, addr) returns a fs_meta, by going and building fs_file and getting its meta
// #new(rb_cTSKFileSystemFileData) gets a given file's meta
// #new(rb_cTSKFileSystemDir) gets that dir's file's meta
VALUE initialize_fs_meta(int argc, VALUE *args, VALUE self){
  VALUE thing; VALUE other;
//  struct tsk4r_fs_file_wrapper * file_ptr;
//  struct tsk4r_fs_meta_wrapper * meta_ptr;
  rb_scan_args(argc, args, "11", &thing, &other);
  
  if (rb_obj_is_kind_of(thing, rb_cTSKFileSystem)) {
    // arg2 an address?
    if (rb_obj_is_kind_of(other, rb_cFixnum)) {
      printf("calling get_meta_from_inum(thing,other)\n");
      rb_funcall(self, rb_intern("get_meta_from_inum"), 2, thing, other);
    } else {
      rb_warn("passed FileSystem::System object, but no address to seek! Arg2 not Fixnum");
    }
  } else if (rb_obj_is_kind_of(thing, rb_cTSKFileSystemFileData)) {
    printf("calling get_meta_from_file(thing)\n");

    rb_funcall(self, rb_intern("get_meta_from_file"), 1, thing);

    
  } else if (rb_obj_is_kind_of(thing, rb_cTSKFileSystemDir)) {
    printf("calling get_meta_from_dir(thing)\n");

    rb_funcall(self, rb_intern("get_meta_from_dir"), 1, thing);

    
  } else {
    rb_warn("FileMeta#new requires FileSystem::System, FileSystem::FileData or FileSystem::Directory as arg1.");
  }
  
//  Data_Get_Struct(thing, struct tsk4r_fs_file_wrapper, file_ptr);
//  Data_Get_Struct(self, struct tsk4r_fs_meta_wrapper, meta_ptr);
//  if ( file_ptr->file->meta ) {
//    meta_ptr->metadata = file_ptr->file->meta;
//    rb_iv_set(self, "@addr", LONG2FIX(meta_ptr->metadata->addr));
//    printf("access to TSK_FS_META successful!\n");
//  } else {
//    rb_warn("access to TSK_FS_FILE struct's meta field failed.");
//  }

  return self;
}

// private functions for accessing TSK_FS_META struct
// this first one makes a temporary TSK_FS_FILE struct to get at TSK_FS_META
VALUE get_meta_from_inum(VALUE self, VALUE filesystem, VALUE addr) {
  printf("get_meta_from_inum responding\n");
  struct tsk4r_fs_wrapper * fs_ptr;
  struct tsk4r_fs_meta_wrapper * meta_ptr;
  TSK_FS_FILE * fs_file;
  
  Data_Get_Struct(filesystem, struct tsk4r_fs_wrapper, fs_ptr);
  Data_Get_Struct(self, struct tsk4r_fs_meta_wrapper, meta_ptr);
  fs_file = tsk_fs_file_open_meta(fs_ptr->filesystem, NULL, (TSK_INUM_T)FIX2LONG(addr));
  
  if ( fs_file->meta ) {
    meta_ptr->metadata = fs_file->meta;
    rb_iv_set(self, "@addr", LONG2FIX(meta_ptr->metadata->addr));
    printf("access to TSK_FS_META (from fs + inum) successful!\n");
  } else {
    rb_warn("access to TSK_FS_FILE struct's meta field failed.");
  }
  return self;
}

VALUE get_meta_from_file(VALUE self, VALUE fs_file) {
  printf("get_meta_from_file responding\n");

  struct tsk4r_fs_file_wrapper * file_ptr;
  struct tsk4r_fs_meta_wrapper * meta_ptr;
  Data_Get_Struct(fs_file, struct tsk4r_fs_file_wrapper, file_ptr);
  Data_Get_Struct(self, struct tsk4r_fs_meta_wrapper, meta_ptr);

  if ( file_ptr->file->meta ) {
    meta_ptr->metadata = file_ptr->file->meta;
    rb_iv_set(self, "@addr", LONG2FIX(meta_ptr->metadata->addr));
    printf("access to TSK_FS_META (from file) successful!\n");
  } else {
    rb_warn("access to TSK_FS_FILE struct's meta field failed.");
  }
  return self;
}

VALUE get_meta_from_dir(VALUE self, VALUE fs_dir)  {
  printf("get_meta_from_dir responding\n");

  struct tsk4r_fs_dir_wrapper * dir_ptr;
  struct tsk4r_fs_meta_wrapper * meta_ptr;
  TSK_FS_FILE * fs_file;
  Data_Get_Struct(fs_dir, struct tsk4r_fs_dir_wrapper, dir_ptr);
  Data_Get_Struct(self, struct tsk4r_fs_meta_wrapper, meta_ptr);
  fs_file = dir_ptr->directory->fs_file;
  
  if ( fs_file->meta ) {
    meta_ptr->metadata = fs_file->meta;
    rb_iv_set(self, "@addr", LONG2FIX(meta_ptr->metadata->addr));
    printf("access to TSK_FS_META (from dir) successful!\n");
  } else {
    rb_warn("access to TSK_FS_FILE struct's meta field failed.");
  }
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
  printf("parsed inum = %llu\n",(TSK_INUM_T)parsed_inum);
  TSK_FS_INFO * system  = ptr->filesystem;
  TSK_FS_FILE * tskfile = fsfile->file;

//  tsk_fs_ffind(system, <#TSK_FS_FFIND_FLAG_ENUM lclflags#>, parsed_inum, TSK_FS_ATTR_TYPE_DEFAULT, <#uint8_t type_used#>, <#uint16_t id#>, <#uint8_t id_used#>, <#TSK_FS_DIR_WALK_FLAG_ENUM flags#>);
  tsk_fs_file_open_meta(system, tskfile, (TSK_INUM_T)parsed_inum);
//
  if (tskfile != NULL) {
    printf("recieved metadata for inum %llu\n", parsed_inum);
    rb_iv_set(self, "@content_len", LONG2FIX(tskfile->meta->content_len));
    rb_iv_set(self, "@address", INT2FIX(tskfile->meta->addr));
    rb_iv_set(self, "@uid", LONG2FIX(tskfile->meta->uid));
  } else {
    rb_warn("unable to perform tsk_fs_file_open_meta");
  }
  return self;

}


void open_fs_name(VALUE self, VALUE fsfile, VALUE inum){
  struct tsk4r_fs_name_wrapper * fsname;
  struct tsk4r_fs_file_wrapper * ptr;
  Data_Get_Struct(self, struct tsk4r_fs_name_wrapper, fsname);
  Data_Get_Struct(fsfile, struct tsk4r_fs_file_wrapper, ptr);
  
  fsname->name = ptr->file->name;
}
