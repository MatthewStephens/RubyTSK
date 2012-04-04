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

// Prototype for the method and classes directly below it
static VALUE rb_mtsk4r;
static VALUE rb_cClass1;
static VALUE rb_cClass2;

// other classes here
static VALUE rb_cClass3;

// Prototype for the initialization method - Ruby calls this, not you
void Init_tsk4r();

// Prototype for our methods - methods are prefixed by 'method_' here (static keyword encapsulates these)
static VALUE method_hello_world(VALUE self);
static VALUE method_testOmethod(VALUE self);
static VALUE method_testCmethod1(VALUE self);
static VALUE method_testCmethod2(VALUE self);
static VALUE method_testMmethod(VALUE self);
static VALUE initialize(VALUE self);
static VALUE image_size(VALUE self);
static VALUE sector_size(VALUE self);
VALUE klass;

struct myHandle {
	TSK_IMG_INFO *image;
};

// alloc & dealloc
static void deallocate2(TSK_IMG_INFO * image){
	//xfree((TSK_IMG_INFO *)image);
	tsk_img_close(image);
}
static void deallocate(struct myHandle * ptr){
	//xfree((TSK_IMG_INFO *)image);
	TSK_IMG_INFO *image = ptr->image;
	tsk_img_close(image);
	xfree(ptr);
}
static VALUE allocate(VALUE klass){
//	TSK_IMG_INFO *image = ALLOC(TSK_IMG_INFO);
//	TSK_IMG_INFO * image = malloc(sizeof(TSK_IMG_INFO));
//	fprintf(stdout, "allocation complete.\n");
//	return Data_Wrap_Struct(klass, 0, 0, image);

	struct myHandle *ptr;
	return Data_Make_Struct(klass, struct myHandle, 0, deallocate, ptr);
	/*
	TSK_IMG_INFO *sval;
	return Data_Make_Struct(klass, TSK_IMG_INFO, 0, deallocate, sval);
	 */
}

// init a FirstClass (Image) object
VALUE initialize(VALUE self){
	char * filename;
	filename = "spec/samples/test.image";
	//TSK_IMG_INFO *image;  // declare inner struct
	//Data_Get_Struct(self, TSK_IMG_INFO, image); // unwrap inner struct from self
	// from here, manipulation of image will be reflected in self
	// access self in other instance methods by using Data_Get_Struct in same fashion as above
//	image = tsk_img_open_sing(filename, TSK_IMG_TYPE_DETECT, 0);
	/*
	DATA_PTR(self) = tsk_img_open_sing(filename, TSK_IMG_TYPE_DETECT, 0);
	 */
	struct myHandle* ptr;
	Data_Get_Struct(self, struct myHandle, ptr);
	ptr->image = tsk_img_open_sing(filename, TSK_IMG_TYPE_DETECT, 0);
	TSK_IMG_INFO *image = ptr->image;
	fprintf(stdout, "sector size: %d\n", (int)image->sector_size );
	int x = (int)image->sector_size;
	rb_iv_set(self, "@sec_size", INT2FIX(x));
	rb_iv_set(self, "@size", INT2FIX((int)image->size));

	//fprintf(stdout, "image size: %d\tsector size:%d\n", (int)image->size, (int)image->sector_size);
/*
	if (image == NULL) {
        fprintf(stderr, "Error opening image %s\n", filename);
		// TO DO: feed tsk error into Ruby exception or warning
        tsk_error_print(stderr);
        return Qfalse;
    }
	else {
		fprintf(stdout, "image %s opened OK.\n", filename);
	}
	*/
	/*
	Check_Type(self, T_DATA);
	// pump stuff into instance vars
	VALUE size;
	size = INT2NUM((int)image->size);
	rb_iv_set(self, "@size", size);
	VALUE sector_size;
	sector_size = INT2NUM((int)image->sector_size);
	rb_iv_set(self, "@sec_size", sector_size);
	fprintf(stdout, "image type: %d\n", (int)image->itype);
	rb_iv_set(self, "@type", INT2NUM((int)image->itype));
	 */
	// done
	fprintf(stdout, "DATA_PTR(self): %lu\n", (long)DATA_PTR(self));
	//DATA_PTR(self) = image;  // this is the UNDOCUMENTED SECRET to exposing the C Struct 'image' to other methods!!
	fprintf(stdout, "DATA_PTR(self): %lu\n", (long)DATA_PTR(self));

	return self;
}

static VALUE image_size(VALUE self){
	TSK_IMG_INFO* image;
	fprintf(stdout, "DATA_PTR(self): %lu\n", (long)DATA_PTR(self));

	Data_Get_Struct(self, TSK_IMG_INFO, image);
	fprintf(stdout, "DATA_PTR(self): %lu\n", (long)DATA_PTR(self));

	//fprintf(stdout, "image size: %d\n", (int)image->size);
	return INT2NUM((int)image->size);
}

static VALUE sector_size(VALUE self){
	struct myHandle * ptr;
	Data_Get_Struct(self, struct myHandle, ptr);
	TSK_IMG_INFO *image=ptr->image;
	//fprintf(stdout, "sector size: %d\n", (int)image->sector_size);
	return INT2NUM(image->sector_size);
}

static VALUE image_type(VALUE self){
	TSK_IMG_INFO *image;
	Data_Get_Struct(self, TSK_IMG_INFO, image);
	return INT2NUM((int)image->itype);
}

// The initialization method for this module
void Init_tsk4r() {
	rb_mtsk4r = rb_define_module("TSK");
	
	// class definitions
	rb_cClass1 = rb_define_class_under(rb_mtsk4r, "Image", rb_cObject);
	rb_cClass2 = rb_define_class_under(rb_mtsk4r, "SecondClass", rb_cObject);
	
	// allocation functions
	rb_define_alloc_func(rb_cClass1, allocate);

	// sub classes
	rb_cClass3 = rb_define_class_under(rb_cClass2, "ThirdClass", rb_cObject);

	
	// some method templates
	// a basic (module) methods -- note special function used to create
	rb_define_module_function(rb_mtsk4r, "module_hello", method_hello_world, 0);
	rb_define_module_function(rb_mtsk4r, "module_features", method_testMmethod, 0);

	// class methods
	rb_define_module_function(rb_cClass1, "class_features", method_testCmethod1, 0);
	rb_define_module_function(rb_cClass2, "class_features", method_testCmethod2, 0);

	// object methods for FirstClass objects
	rb_define_method(rb_cClass1, "object_method_sample", method_testOmethod, 1); // change arg1 to klass?
	rb_define_method(rb_cClass1, "object_hello", method_hello_world, 0); // change arg1 to klass?
	rb_define_method(rb_cClass1, "initialize", initialize, 0);
	rb_define_method(rb_cClass1, "image_size", image_size, 0);
	rb_define_method(rb_cClass1, "sector_size", sector_size, 0);
	rb_define_method(rb_cClass1, "image_type", image_type, 0);

	// attributes (read only)
	rb_define_attr(rb_cClass1, "size", 1, 0);
	rb_define_attr(rb_cClass1, "sec_size", 1, 0);
	rb_define_attr(rb_cClass1, "type", 1, 0);

	
	// object methods for SecondClass objects
	rb_define_method(rb_cClass2, "object_method_sample", method_testOmethod, 1); // change arg1 to klass?

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
	return rb_str_new2("This is class method 2 responding.  I was assigned to SecondClass");
}

VALUE method_testOmethod(VALUE klass)
{
	return rb_str_new2("I am simply an object method.");
}


