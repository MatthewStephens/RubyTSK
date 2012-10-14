/*
 *  fs_file.c: File classes for tsk4r
 *
 *  tsk4r: The SleuthKit 4 Ruby
 *
 *  Created by Matthew H. Stephens
 *  Copyright 2011,2012 Matthew H. Stephens. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Binding to the SleuthKit (libtsk3) library copyright 2003,2012 by Brian Carrier
 */

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
void build_attributes(VALUE self, TSK_FS_META * ptr);


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
//  klassify(fs, "fs");
//  klassify(reference, "reference");
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
    struct tsk4r_fs_dir_wrapper * fs_dir;
    Data_Get_Struct(reference, struct tsk4r_fs_dir_wrapper, fs_dir);
    addr = fs_dir->directory->addr;

    printf("accessing file entry for (%llu) (from a directory object)\n", addr);
    // if directory already built, we should have TSK_FS_FILE pointer
    fs_file->file = fs_dir->directory->fs_file;
    
  } else if (rb_obj_is_kind_of(reference, rb_cFixnum)) {

    addr = (TSK_INUM_T)FIX2ULONG(reference);  TSK_FS_FILE * fs_temp_file;

    fs_temp_file = tsk_fs_file_open_meta(filesystem, NULL, addr);
    if (fs_temp_file != NULL ) { fs_file->file = fs_temp_file; }

  } else if (rb_obj_is_kind_of(reference, rb_cString)) {

    TSK_TCHAR * name; TSK_FS_FILE * fs_temp_file;
    name = StringValuePtr(reference);
    printf("calling tsk_fs_file_open('%s')\n", name);
    fs_temp_file = tsk_fs_file_open(filesystem, NULL, name);
    if (fs_temp_file != NULL ) { fs_file->file = fs_temp_file; }

  } else {
    rb_warn("arg2 should be a FileSystem::Directory object, Fixnum, or String!");
  }

  if (fs_file->file != NULL) {
    rb_iv_set(self, "@meta", rb_funcall(rb_cTSKFileSystemFileMeta, rb_intern("new"), 1, self));
    // build FileName if possible
    if (fs_file->file->name != NULL) {
      rb_iv_set(self, "@name", rb_funcall(rb_cTSKFileSystemFileName, rb_intern("new"), 1, self));
    }
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
      rb_funcall(self, rb_intern("get_meta_from_inum"), 2, thing, other);
    } else {
      rb_warn("passed FileSystem::System object, but no address to seek! Arg2 not Fixnum");
    }
  } else if (rb_obj_is_kind_of(thing, rb_cTSKFileSystemFileData)) {
    rb_funcall(self, rb_intern("get_meta_from_file"), 1, thing);

  } else if (rb_obj_is_kind_of(thing, rb_cTSKFileSystemDir)) {
    rb_funcall(self, rb_intern("get_meta_from_dir"), 1, thing);

  } else {
    rb_warn("FileMeta#new requires FileSystem::System, FileSystem::FileData or FileSystem::Directory as arg1.");
  }

  return self;
}

// private functions for accessing TSK_FS_META struct
// this first one makes a temporary TSK_FS_FILE struct to get at TSK_FS_META
VALUE get_meta_from_inum(VALUE self, VALUE filesystem, VALUE addr) {
  struct tsk4r_fs_wrapper * fs_ptr;
  struct tsk4r_fs_meta_wrapper * meta_ptr;
  TSK_FS_FILE * fs_file;
  
  Data_Get_Struct(filesystem, struct tsk4r_fs_wrapper, fs_ptr);
  Data_Get_Struct(self, struct tsk4r_fs_meta_wrapper, meta_ptr);
  fs_file = tsk_fs_file_open_meta(fs_ptr->filesystem, NULL, (TSK_INUM_T)FIX2LONG(addr));
  
  if ( fs_file->meta ) {
    meta_ptr->metadata = fs_file->meta;
    build_attributes(self, meta_ptr->metadata);

  } else {
    rb_warn("access to TSK_FS_FILE struct's meta field failed.");
  }
  return self;
}

VALUE get_meta_from_file(VALUE self, VALUE fs_file) {
  struct tsk4r_fs_file_wrapper * file_ptr;
  struct tsk4r_fs_meta_wrapper * meta_ptr;
  Data_Get_Struct(fs_file, struct tsk4r_fs_file_wrapper, file_ptr);
  Data_Get_Struct(self, struct tsk4r_fs_meta_wrapper, meta_ptr);

  if ( file_ptr->file->meta ) {
    meta_ptr->metadata = file_ptr->file->meta;
    build_attributes(self, meta_ptr->metadata);

  } else {
    rb_warn("access to TSK_FS_FILE struct's meta field failed.");
  }
  return self;
}

VALUE get_meta_from_dir(VALUE self, VALUE fs_dir)  {
  struct tsk4r_fs_dir_wrapper * dir_ptr;
  struct tsk4r_fs_meta_wrapper * meta_ptr;
  TSK_FS_FILE * fs_file;
  Data_Get_Struct(fs_dir, struct tsk4r_fs_dir_wrapper, dir_ptr);
  Data_Get_Struct(self, struct tsk4r_fs_meta_wrapper, meta_ptr);
  fs_file = dir_ptr->directory->fs_file;
  
  if ( fs_file->meta ) {
    meta_ptr->metadata = fs_file->meta;
    build_attributes(self, meta_ptr->metadata);

  } else {
    rb_warn("access to TSK_FS_FILE struct's meta field failed.");
  }
  return self;
}

VALUE get_number_of_attributes(VALUE self) {
  VALUE number; int n; TSK_FS_FILE * fs_file; struct tsk4r_fs_file_wrapper * fwrapper;
  Data_Get_Struct(self, struct tsk4r_fs_file_wrapper, fwrapper);
  fs_file = fwrapper->file;
  n = tsk_fs_file_attr_getsize(fs_file);
  number = INT2NUM(n);
  return number;
}

void build_attributes(VALUE self, TSK_FS_META * metadata) {

  rb_iv_set(self, "@addr", LONG2FIX(metadata->addr));
  rb_iv_set(self, "@atime", LONG2FIX(metadata->atime));
  rb_iv_set(self, "@atime_nano", LONG2FIX(metadata->atime_nano));
//  rb_iv_set(self, "@attr", LONG2FIX(metadata->attr));
//  rb_iv_set(self, "@attr_state", LONG2FIX(metadata->attr_state));
  rb_iv_set(self, "@content_len", LONG2FIX(metadata->content_len));
  rb_iv_set(self, "@content_ptr", LONG2FIX(metadata->content_ptr));
  rb_iv_set(self, "@crtime", LONG2FIX(metadata->crtime));
  rb_iv_set(self, "@crtime_nano", LONG2FIX(metadata->crtime_nano));
  rb_iv_set(self, "@ctime", LONG2FIX(metadata->ctime));
  rb_iv_set(self, "@ctime_nano", LONG2FIX(metadata->ctime_nano));
//  rb_iv_set(self, "@flags", LONG2FIX(metadata->flags));
  rb_iv_set(self, "@gid", INT2FIX(metadata->gid));
  rb_iv_set(self, "@link", LONG2FIX(metadata->link));
  rb_iv_set(self, "@mode", LONG2FIX(metadata->mode));
  rb_iv_set(self, "@mtime", LONG2FIX(metadata->mtime));
  rb_iv_set(self, "@mtime_nano", LONG2FIX(metadata->mtime_nano));
//  rb_iv_set(self, "@name2", LONG2FIX(metadata->name2));
  rb_iv_set(self, "@nlink", LONG2FIX(metadata->nlink));
  rb_iv_set(self, "@seq", LONG2FIX(metadata->seq));
  rb_iv_set(self, "@size", LONG2FIX(metadata->size));
  rb_iv_set(self, "@tag", INT2FIX(metadata->tag));

}


VALUE initialize_fs_name(int argc, VALUE *args, VALUE self){
  VALUE file_obj; VALUE opts;
  rb_scan_args(argc, args, "11", &file_obj, &opts);
  rb_iv_set(self, "@file", file_obj);
  
  struct tsk4r_fs_file_wrapper * file_ptr;
  struct tsk4r_fs_name_wrapper * my_ptr;
  Data_Get_Struct(file_obj, struct tsk4r_fs_file_wrapper, file_ptr);
  Data_Get_Struct(self, struct tsk4r_fs_name_wrapper, my_ptr);

  if (file_ptr->file->name != NULL) {
    my_ptr->name = file_ptr->file->name;
    if (my_ptr->name) {
      TSK_FS_NAME * name = my_ptr->name;
      rb_iv_set(self, "@meta_addr", LONG2FIX(name->meta_addr));
      rb_iv_set(self, "@meta_seq", LONG2FIX(name->meta_seq));
      rb_iv_set(self, "@name", rb_str_new2(name->name));
      rb_iv_set(self, "@name_size", LONG2FIX(name->name_size));
      rb_iv_set(self, "@parent_addr", LONG2FIX(name->par_addr));
      rb_iv_set(self, "@shrt_name", rb_str_new2(name->shrt_name));
      rb_iv_set(self, "@shrt_name_size", LONG2FIX(name->shrt_name_size));
      rb_iv_set(self, "@tag", INT2FIX(name->tag));

    } else { rb_warn("file_ptr->file had NULL for its name struct."); }
  } else {
    rb_warn("unable to access file name data!");
  }
  
  return self;
}


// private functions
VALUE open_fs_file(int argc, VALUE *args, VALUE self){
  VALUE inum;
  rb_scan_args(argc, args, "10", &inum);

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

  tsk_fs_file_open_meta(system, tskfile, (TSK_INUM_T)parsed_inum);
//
  if (tskfile != NULL) {
    rb_iv_set(self, "@content_len", LONG2FIX(tskfile->meta->content_len));
    rb_iv_set(self, "@address", INT2NUM(tskfile->meta->addr));
    rb_iv_set(self, "@uid", LONG2FIX(tskfile->meta->uid));
  } else {
    rb_warn("unable to perform tsk_fs_file_open_meta");
  }
  return self;

}

