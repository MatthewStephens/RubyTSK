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



static VALUE close_volume(VALUE self){
  return Qnil;
}

static VALUE read_volume_block(int argc, VALUE *args, VALUE self){
  return Qnil;
}

static VALUE walk_volume(VALUE self){
  return Qnil;
}



// The initialization method for this module
void Init_tsk4r() {
  rb_mtsk4r = rb_define_module("Sleuthkit");
  
  // class definitions
  rb_cTSKImage = rb_define_class_under(rb_mtsk4r, "Image", rb_cObject);
  rb_cTSKVolume = rb_define_class_under(rb_mtsk4r, "Volume", rb_cObject);
  rb_cTSKFileSystem = rb_define_class_under(rb_mtsk4r, "FileSystem", rb_cObject);
  
  // allocation functions
  rb_define_alloc_func(rb_cTSKImage, allocate_image);
  rb_define_alloc_func(rb_cTSKVolume, allocate_volume);
  rb_define_alloc_func(rb_cTSKFileSystem, allocate_filesystem);


  // sub classes
  //rb_cTSKFileSystem = rb_define_class_under(rb_cTSKVolume, "ThirdClass", rb_cObject);

  
  // some method templates
  // a basic (module) methods -- note special function used to create
  rb_define_module_function(rb_mtsk4r, "module_hello", method_hello_world, 0);
  rb_define_module_function(rb_mtsk4r, "module_features", method_testMmethod, 0);

  // class methods
  rb_define_module_function(rb_cTSKImage, "class_features", method_testCmethod1, 0);
  rb_define_module_function(rb_cTSKVolume, "class_features", method_testCmethod2, 0);

  // object methods for FirstClass objects
  rb_define_method(rb_cTSKImage, "object_method_sample", method_testOmethod, 1); // change arg1 to klass?
  rb_define_method(rb_cTSKImage, "object_hello", method_hello_world, 0); // change arg1 to klass?
  rb_define_method(rb_cTSKImage, "initialize", initialize_disk_image, -1);
  rb_define_method(rb_cTSKImage, "image_open", image_open, 1);
  rb_define_method(rb_cTSKImage, "image_size", image_size, 0);
  rb_define_method(rb_cTSKImage, "sector_size", sector_size, 0);
  rb_define_method(rb_cTSKImage, "image_type", image_type, 0);

  // attributes (read only)
  rb_define_attr(rb_cTSKImage, "size", 1, 0);
  rb_define_attr(rb_cTSKImage, "sec_size", 1, 0);
  rb_define_attr(rb_cTSKImage, "type", 1, 0);

  
  /* Sleuthkit::Volume */
  // object methods for Volume objects
  rb_define_method(rb_cTSKVolume, "initialize", initialize_volume, -1);
  rb_define_method(rb_cTSKVolume, "open", open_volume, 1); // change arg1 to klass?
  rb_define_method(rb_cTSKVolume, "close", close_volume, 1);
  rb_define_method(rb_cTSKVolume, "read_block", read_volume_block, 3); //read block given start and no. of blocks
  rb_define_method(rb_cTSKVolume, "walk", walk_volume, 1);
  
  // attributes
  rb_define_attr(rb_cTSKVolume, "partition_count", 1, 0);
  rb_define_attr(rb_cTSKVolume, "endian", 1, 0);
  rb_define_attr(rb_cTSKVolume, "offset", 1, 0);
  rb_define_attr(rb_cTSKVolume, "block_size", 1, 0);
    
    /* Sleuthkit::FileSystem */
    // object methods for FileSystem objects
    rb_define_method(rb_cTSKFileSystem, "initialize", initialize_filesystem, -1);
    rb_define_method(rb_cTSKFileSystem, "open", open_filesystem, 1); // change arg1 to klass?

    
    // attributes
    rb_define_attr(rb_cTSKFileSystem, "root_inum", 1, 0);
    rb_define_attr(rb_cTSKFileSystem, "last_inum", 1, 0);
    rb_define_attr(rb_cTSKFileSystem, "block_size", 1, 0);
    rb_define_attr(rb_cTSKFileSystem, "endian", 1, 0);
    rb_define_attr(rb_cTSKFileSystem, "offset", 1, 0);
    rb_define_attr(rb_cTSKFileSystem, "inum_count", 1, 0);
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


