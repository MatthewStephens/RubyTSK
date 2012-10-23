/*
 *  fs_dir.c: Directory classes for tsk4r
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
#include "fs_dir.h"


extern VALUE rb_cTSKImage;
extern VALUE rb_cTSKVolumeSystem;
extern VALUE rb_cTSKVolumePart;
extern VALUE rb_cTSKFileSystem;
extern VALUE rb_cTSKFileSystemDir;
extern VALUE rb_cTSKFileSystemFileData;

extern void klassify();


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
  VALUE source_obj; VALUE reference; VALUE opts;
  rb_scan_args(argc, args, "21", &source_obj, &reference, &opts);

  if ( ! rb_obj_is_kind_of(opts, rb_cHash) ){
    opts = rb_hash_new();
  }
//  opts = rb_funcall(rb_cTSKFileSystemDir, rb_intern("parse_opts"), 1, opts);
  printf("initialize_fs_dir options parsed\n");
  if (rb_obj_is_kind_of(reference, rb_cFixnum) || rb_obj_is_kind_of(reference, rb_cString)) {
    
    rb_funcall(self, rb_intern("open_fs_directory"), 3, source_obj, reference, opts);
    
  } else {
    rb_warn("Sleuthkit::FileSystem::Directory#init did not get String or Fixnum as arg2!!");
  }

  if ( rb_iv_get(self, "@names_used") == Qnil ) {
    rb_warn("could not access directory.");
    rb_funcall(self, rb_intern("taint"), 0,NULL);
  } else {
    rb_iv_set(self, "@parent", source_obj);
  }
  return self;
}

VALUE open_fs_directory(VALUE self, VALUE source_obj, VALUE reference, VALUE opts) {
  struct tsk4r_fs_dir_wrapper * dir_ptr;
  struct tsk4r_fs_wrapper * fs_ptr;
  Data_Get_Struct(self, struct tsk4r_fs_dir_wrapper, dir_ptr);
  Data_Get_Struct(source_obj, struct tsk4r_fs_wrapper, fs_ptr);
  klassify(self, "self");
  klassify(source_obj, "source_obj");
  klassify(reference, "reference");

  
  if (rb_obj_is_kind_of(reference, rb_cString)) {
    dir_ptr->directory = tsk_fs_dir_open(fs_ptr->filesystem, StringValuePtr(reference));
    if (dir_ptr->directory == NULL) {
      printf("opened dir, got NULL, returning to init.\n");
    }
  }
  else if (rb_obj_is_kind_of(reference, rb_cFixnum)) {
    TSK_INUM_T addr = (TSK_INUM_T)FIX2ULONG(reference);
    dir_ptr->directory = tsk_fs_dir_open_meta(fs_ptr->filesystem, addr);
    if (dir_ptr->directory == NULL) printf("opened dir, got NULL, returning to init.\n");
  } else {
      rb_warn("arg2 is not a String or Fixnum!");
  }
  // populate object attributes
  if (dir_ptr->directory != NULL) {
    rb_iv_set(self, "@inum", LONG2FIX(dir_ptr->directory->addr));
    rb_iv_set(self, "@names_used", LONG2FIX(dir_ptr->directory->names_used));
    rb_iv_set(self, "@names_alloc", LONG2FIX(dir_ptr->directory->names_alloc));
    
    VALUE names = rb_ary_new2(dir_ptr->directory->names_used);
    TSK_FS_NAME * names_list = dir_ptr->directory->names;
    int c;
    for (c=0; c < dir_ptr->directory->names_used; c++) {
      char * n_str = names_list[c].name;
      rb_ary_push(names, rb_str_new2(n_str));
    }
    rb_iv_set(self, "@names", names);
    
    // build a new file object for the directory
    // FileData#new(fs, dir)
    VALUE directory_file = rb_funcall(rb_cTSKFileSystemFileData, rb_intern("new"), 2, source_obj, self);
    rb_iv_set(self, "@file", directory_file);
  }
  return self;
}

VALUE get_size(VALUE self) {
  VALUE size; struct tsk4r_fs_dir_wrapper * dir_wrapper; TSK_FS_DIR * dir;
  Data_Get_Struct(self, struct tsk4r_fs_dir_wrapper, dir_wrapper);
  dir = dir_wrapper->directory;
  unsigned long val = tsk_fs_dir_getsize(dir);
  size = ULONG2NUM(val);
  return size;
}