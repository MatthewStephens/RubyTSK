/*
 *  tsk4r.c
 *  RubyTSK
 *
 *  Created by ms3uf on 5/1/11.
 *  Copyright 2011 Matthew H. Stephens. 
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
#include <ctype.h>
#include <ruby.h>
#include "tsk4r.h"
#include <tsk3/libtsk.h>
#include <tsk3/img/tsk_img_i.h>
#include "tsk3/img/img_types.c"
//#include "sleuthkit-3.2.1/tsk3/libtsk.h"

static VALUE rb_mTSK;
static VALUE rb_cTSKBase;
static VALUE rb_cTSKDiskImage;
static VALUE rb_cTSKVolume;
static VALUE rb_cTSKFileSystem;
static VALUE rb_cTSKHashDatabase;
static VALUE rb_cTSKTools; // class for binding to tsk CLI binaries
static VALUE rb_cTSKImgMap;
int some_val; // just for testing

// Defining a space for information and references about the module to be stored internally
VALUE tsk4r = Qnil;

// Prototype for the initialization method - Ruby calls this, not you
void Init_tsk4r();

// Prototype for our method 'test1' - methods are prefixed by 'method_' here
VALUE method_test1(VALUE self);
VALUE method_tsk_version_const(VALUE self);
VALUE method_tsk_version(VALUE self);
VALUE method_tsk_di_img_stat(VALUE self, char * dname);
VALUE method_tsk_img_type_supported();

// classes

// The initialization method for this module
void Init_tsk4r() {
	rb_mTSK = rb_define_module("TSK");
	
	// class definitions
	
	rb_cTSKBase = rb_define_class_under(rb_mTSK, "Base", rb_cObject);
	rb_cTSKDiskImage = rb_define_class_under(rb_mTSK, "DiskImage", rb_cObject);
	rb_cTSKVolume = rb_define_class_under(rb_mTSK, "Volume", rb_cObject);
	rb_cTSKFileSystem = rb_define_class_under(rb_mTSK, "FileSystem", rb_cObject);
	rb_cTSKHashDatabase = rb_define_class_under(rb_mTSK, "HashDatabase", rb_cObject);
	rb_cTSKTools = rb_define_class_under(rb_mTSK, "Tools", rb_cObject);
	rb_cTSKImgMap = rb_define_class_under(rb_mTSK, "ImgMap", rb_cObject);
	
	// basic (module) methods
	
	rb_define_method(rb_mTSK, "test1", method_test1, 0);
	rb_define_method(rb_mTSK, "tsk_version_const", method_tsk_version_const, 0);
	rb_define_method(rb_mTSK, "tsk_version", method_tsk_version, 0);
	
	// methods for class DiskImage
	rb_define_method(rb_cTSKDiskImage, "img_stat", method_tsk_di_img_stat, 1); // change arg1 to klass?
	// class methods
	rb_define_module_function(rb_cTSKDiskImage, "img_types", method_tsk_img_type_supported, 0);
	
	// should put tsk version in global constant
	rb_define_const(rb_mTSK, "TSKVersion", rb_str_new2(TSK_VERSION_STR));
	rb_define_const(rb_mTSK, "Version", rb_str_new2(VERSION));

}

// class methods for Disk Image
VALUE method_tsk_di_img_stat(VALUE self, char * dname) { // no. of args, args, reciever?
	tsk_img_open_sing(dname, TSK_IMG_TYPE_DETECT, 0);
	printf("opening %s\n", dname); 
	return Qnil;
}

VALUE method_tsk_img_map_new() {
	extern IMG_TYPES img_open_table[]; 
	int i, len;
	len=sizeof(img_open_table)/sizeof(int);
	printf("len: %d\n", len);
	IMG_TYPES row;
	i=0;
//	VALUE myhash;
	row = img_open_table[i]; // name, code, comment
	VALUE result;
	result = rb_hash_new();
	char *key = "name";
	char *value;
	IMG_TYPES iot = img_open_table[0];
	value = iot.name;
	printf("value: %s\n", value);
	rb_hash_aset(result, (VALUE) key, (VALUE) value);
//	myhash = Data_Wrap_Struct(T_HASH, 0, -1, &row);
	return result;
}

VALUE method_tsk_img_type_supported() {
	extern IMG_TYPES img_open_table[]; 
	int len;
	len=(sizeof(img_open_table)/sizeof(int))-1;
	VALUE image_map;
	image_map = rb_ary_new();
    IMG_TYPES *sp;
	for ( sp = img_open_table; sp->name; sp++) {
			VALUE result = rb_hash_new();
			char * name = (*sp).name;
			uint8_t code = (*sp).code;
			char * comment = (*sp).comment;
			rb_hash_aset(result, rb_str_new2("name"), rb_str_new2(name));
			rb_hash_aset(result, rb_str_new2("code"), INT2NUM((int) code));
			rb_hash_aset(result, rb_str_new2("description"), rb_str_new2(comment));
			rb_ary_push(image_map, result);
					
		}
	
	return image_map;
}

// Our 'test1' method.. it simply returns a value of '10' for now.
VALUE method_test1(VALUE self) {
	int x = 10;
	return INT2NUM(x);
}

// these two methods bind to a base constant and a simple function
VALUE method_tsk_version_const(VALUE self) {
	printf("TSK_VERSION_STR: %s\n", TSK_VERSION_STR);
	return Qtrue;
}

VALUE method_tsk_version(VALUE self) {
	const char * v = tsk_version_get_str();
	printf("TSK Version: %s\n",  v);
    return Qtrue;
}
