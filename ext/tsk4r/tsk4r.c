/*
 *  tsk4r.c
 *  tsk4r
 *
 *  Created by Matthew H. Stephens on 9/14/11.
 *  Copyright 2011 Matthew H. Stephens.  All rights reserved. 
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
 */

#include <stdio.h>
#include <tsk3/libtsk.h>
#include <ruby.h>
#include "tsk4r.h"

// method prototypes


// Prototype for our methods - methods are prefixed by 'method_' here (static keyword encapsulates these)
static VALUE method_hello_world(VALUE self);
static VALUE method_testOmethod(VALUE self);
static VALUE method_testCmethod1(VALUE self);
static VALUE method_testCmethod2(VALUE self);
static VALUE method_testMmethod(VALUE self);



VALUE klass;



// alloc & dealloc



static VALUE close_volume_system(VALUE self){
  return Qnil;
}

static VALUE read_volume_block(int argc, VALUE *args, VALUE self){
  return Qnil;
}

static VALUE walk_volume(VALUE self){
  return Qnil;
}

// constants
#define TSK4R_FS_ATTRS_COUNT 24
const char * TSK4R_FS_ATTRS[TSK4R_FS_ATTRS_COUNT] = { "block_count",
  "block_post_size",
  "block_pre_size",
  "block_size",
  "dev_bsize",
  "data_unit_name",
  "endian",
  "first_inum",
  "first_block",
  "flags",
  "fs_id",
  "fs_id_used",
  "ftype",
  "inum_count",
  "isOrphanHunting",
  "journ_inum",
  "last_block",
  "last_block_act",
  "last_inum",
  "list_inum_named",
  "offset",
  "orphan_dir",
  "root_inum",
  "tag" };



// The initialization method for this module
void Init_tsk4r() {
  rb_mtsk4r = rb_define_module("Sleuthkit");
  
  rb_const_set(rb_mtsk4r, rb_intern("TSK_VERSION"), rb_str_new2(tsk_version_get_str()));
  
  // class definitions
  rb_cTSKImage           = rb_define_class_under(rb_mtsk4r, "Image", rb_cObject);
  rb_cTSKVolumeSystem    = rb_define_class_under(rb_mtsk4r, "VolumeSystem", rb_cObject);
  rb_cTSKVolumePart      = rb_define_class_under(rb_mtsk4r, "VolumePart", rb_cObject);
  rb_cTSKFileSystem      = rb_define_class_under(rb_mtsk4r, "FileSystem", rb_cObject);
  rb_cTSKFileSystemDir   = rb_define_class_under(rb_mtsk4r, "FileSystemDirectory", rb_cObject);
  rb_cTSKFileSystemFileData   = rb_define_class_under(rb_mtsk4r, "FileSystemFileData", rb_cObject);
  rb_cTSKFileSystemFileMeta   = rb_define_class_under(rb_mtsk4r, "FileSystemFileMeta", rb_cObject);
  rb_cTSKFileSystemFileName   = rb_define_class_under(rb_mtsk4r, "FileSystemFileName", rb_cObject);

  
  // allocation functions
  rb_define_alloc_func(rb_cTSKImage, allocate_image);
  rb_define_alloc_func(rb_cTSKVolumeSystem, allocate_volume_system);
  rb_define_alloc_func(rb_cTSKVolumePart, allocate_volume_part);
  rb_define_alloc_func(rb_cTSKFileSystem, allocate_filesystem);
  rb_define_alloc_func(rb_cTSKFileSystemDir, allocate_fs_dir);
  rb_define_alloc_func(rb_cTSKFileSystemFileData, allocate_fs_file);
  rb_define_alloc_func(rb_cTSKFileSystemFileMeta, allocate_fs_meta);
  rb_define_alloc_func(rb_cTSKFileSystemFileName, allocate_fs_name);


  // sub classes
  //rb_cTSKFileSystem = rb_define_class_under(rb_cTSKVolume, "ThirdClass", rb_cObject);

  
  // some method templates
  // a basic (module) methods -- note special function used to create
  rb_define_module_function(rb_mtsk4r, "module_hello", method_hello_world, 0);
  rb_define_module_function(rb_mtsk4r, "module_features", method_testMmethod, 0);

  // class methods
  rb_define_module_function(rb_cTSKImage, "class_features", method_testCmethod1, 0);
  rb_define_module_function(rb_cTSKVolumeSystem, "class_features", method_testCmethod2, 0);

  // object methods for Sleuthkit::Image objects
  rb_define_method(rb_cTSKImage, "object_method_sample", method_testOmethod, 1); // change arg1 to klass?
  rb_define_method(rb_cTSKImage, "object_hello", method_hello_world, 0); // change arg1 to klass?
  rb_define_method(rb_cTSKImage, "initialize", initialize_disk_image, -1);
  rb_define_method(rb_cTSKImage, "image_open", image_open, 2);

  // attributes (read only)
  rb_define_attr(rb_cTSKImage, "path", 1, 0);
  rb_define_attr(rb_cTSKImage, "size", 1, 0);
  rb_define_attr(rb_cTSKImage, "sector_size", 1, 0);
  rb_define_attr(rb_cTSKImage, "type", 1, 0);
  rb_define_attr(rb_cTSKImage, "description", 1, 0);
  rb_define_attr(rb_cTSKImage, "name", 1, 0);
  rb_define_attr(rb_cTSKImage, "auto_detect", 1, 0);

  
  /* Sleuthkit::Volume */
  // object methods for VolumeSystem objects
  rb_define_method(rb_cTSKVolumeSystem, "initialize", initialize_volume_system, -1);
  rb_define_method(rb_cTSKVolumeSystem, "open", open_volume_system, 2); // change arg1 to klass?
  rb_define_method(rb_cTSKVolumeSystem, "close", close_volume_system, 1);
  rb_define_method(rb_cTSKVolumeSystem, "read_block", read_volume_block, 3); //read block given start and no. of blocks
  rb_define_method(rb_cTSKVolumeSystem, "walk", walk_volume, 1);
  rb_define_method(rb_cTSKVolumeSystem, "expose_part", volume_expose_part, 0);
  rb_define_method(rb_cTSKVolumeSystem, "expose_part_at", volume_expose_part_by_idx, 1);
  rb_define_private_method(rb_cTSKVolumeSystem, "get_partitions", volume_get_partitions, 1);
  
  // attributes
  rb_define_attr(rb_cTSKVolumeSystem, "partition_count", 1, 0);
  rb_define_attr(rb_cTSKVolumeSystem, "endian", 1, 0);
  rb_define_attr(rb_cTSKVolumeSystem, "offset", 1, 0);
  rb_define_attr(rb_cTSKVolumeSystem, "block_size", 1, 0);
  rb_define_attr(rb_cTSKVolumeSystem, "description", 1, 0);
  rb_define_attr(rb_cTSKVolumeSystem, "volume_system_type", 1, 0);
  rb_define_attr(rb_cTSKVolumeSystem, "parts", 1, 0);
  rb_define_attr(rb_cTSKVolumeSystem, "partition_one", 1, 0);
  rb_define_attr(rb_cTSKVolumeSystem, "parent", 1, 0);


  
  // object methods for VolumePart objects
  rb_define_method(rb_cTSKVolumePart, "initialize", initialize_volume_part, -1);
  rb_define_method(rb_cTSKVolumePart, "open", open_volume_part, -1); // change arg1 to klass?
  
//  rb_define_method(rb_cTSKVolumePart, "read_block", read_volume_part_block, 3); //read block given start and no. of blocks
//  rb_define_method(rb_cTSKVolumePart, "walk", walk_volume_part, 1);
    
  // attributes
  rb_define_attr(rb_cTSKVolumePart, "start", 1, 0);
  rb_define_attr(rb_cTSKVolumePart, "length", 1, 0);
  rb_define_attr(rb_cTSKVolumePart, "description", 1, 0);
  rb_define_attr(rb_cTSKVolumePart, "table_number", 1, 0);
  rb_define_attr(rb_cTSKVolumePart, "slot_number", 1, 0);
  rb_define_attr(rb_cTSKVolumePart, "address", 1, 0);
  rb_define_attr(rb_cTSKVolumePart, "flags", 1, 0);
  rb_define_attr(rb_cTSKVolumePart, "parent", 1, 0);
  rb_define_attr(rb_cTSKVolumePart, "next", 1, 0);
  rb_define_attr(rb_cTSKVolumePart, "prev", 1, 0);
  
  /* Sleuthkit::FileSystem */
  // object methods for FileSystem objects
  rb_define_method(rb_cTSKFileSystem, "initialize", initialize_filesystem, -1);
  rb_define_method(rb_cTSKFileSystem, "open", open_filesystem, -1); // change arg1 to klass?
  rb_define_method(rb_cTSKFileSystem, "system_name", get_filesystem_type, 0);
  rb_define_method(rb_cTSKFileSystem, "call_tsk_fsstat", call_tsk_fsstat, 1);
  rb_define_method(rb_cTSKFileSystem, "open_directory_by_name", open_directory_by_name, -1);
  rb_define_method(rb_cTSKFileSystem, "open_directory_by_inum", open_directory_by_inum, -1);
  
  // attributes based on TSK struct

  int i;
  for (i = 0; i < TSK4R_FS_ATTRS_COUNT; i++) {
    rb_define_attr(rb_cTSKFileSystem, TSK4R_FS_ATTRS[i], 1, 0);
  }
  // extra attributes
  rb_define_attr(rb_cTSKFileSystem, "name", 1, 0);
  rb_define_attr(rb_cTSKFileSystem, "description", 1, 0);
  rb_define_attr(rb_cTSKFileSystem, "parent", 1, 0);
  
  
  /* Sleuthkit::FileSystemDirectory */
  // object methods for FileSystemDirectory objects
  rb_define_method(rb_cTSKFileSystemDir, "initialize", initialize_fs_dir, -1);
  
  // attributes
  rb_define_attr(rb_cTSKFileSystemDir, "parent", 1, 0);
  

  /* Sleuthkit::FileSystemFileData */
  
  // object methods for FileSystemFileData objects
  rb_define_method(rb_cTSKFileSystemFileData, "initialize", initialize_fs_file, -1);
  rb_define_method(rb_cTSKFileSystemFileData, "open_fs_file", open_fs_file, -1);
  // attributes
  rb_define_attr(rb_cTSKFileSystemFileData, "parent", 1, 0);
  rb_define_attr(rb_cTSKFileSystemFileData, "content_len", 1, 0);
  rb_define_attr(rb_cTSKFileSystemFileData, "address", 1, 0);

  
  /* Sleuthkit::FileSystemFileMeta */
  
  // object methods for FileSystemFileMeta objects
  rb_define_method(rb_cTSKFileSystemFileMeta, "initialize", initialize_fs_meta, -1);
  
  // attributes
  rb_define_attr(rb_cTSKFileSystemFileMeta, "parent", 1, 0);
  
  /* Sleuthkit::FileSystemFileName */
  
  // object methods for FileSystemFileName objects
  rb_define_method(rb_cTSKFileSystemFileName, "initialize", initialize_fs_name, -1);
  
  // attributes
  rb_define_attr(rb_cTSKFileSystemFileName, "parent", 1, 0);


}

// methods follow here

VALUE method_hello_world(VALUE klass)
{
  return rb_str_new2("hello world");
}

VALUE method_testMmethod(VALUE klass)
{
  return rb_str_new2("This is a module method responding.");
}

VALUE method_testCmethod1(VALUE klass)
{
  return rb_str_new2("This is class method 1 responding.  I was assigned to Image");
}

VALUE method_testCmethod2(VALUE klass)
{
  return rb_str_new2("This is class method 2 responding.  I was assigned to Volume");
}

VALUE method_testOmethod(VALUE klass)
{
  return rb_str_new2("I am simply an object method.");
}


