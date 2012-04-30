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
// Sleuthkit::Image
static VALUE initialize_disk_image(int argc, VALUE *args, VALUE self);
static VALUE image_open(VALUE self, VALUE filename_str);
static VALUE image_size(VALUE self);
static VALUE sector_size(VALUE self);
// Sleuthkit::Volume
static VALUE initialize_volume(int argc, VALUE *args, VALUE self);
static VALUE open_volume(VALUE self, VALUE image_obj);
static VALUE close_volume(VALUE self);
static VALUE read_volume_block(int argc, VALUE *args, VALUE self);
static VALUE walk_volume(VALUE self);
// Sleuthkit::FileSystem
static VALUE initialize_filesystem(int argc, VALUE *args, VALUE self);
static VALUE open_filesystem(VALUE self, VALUE image_obj);
static VALUE open_filesystem_from_vol(VALUE self, VALUE vol_obj);
static VALUE close_filesystem(VALUE self);


VALUE klass;

// Sleuthkit::Image struct
static struct tsk4r_wrapper {
	TSK_IMG_INFO * image;
	char * fn_given;
}tsk4r_wrapper;
// Sleuthkit::Volume struct
static struct tsk4r_vs_wrapper {
	TSK_VS_INFO * volume;
//	tsk4r_wrapper * disk;
}tsk4r_vs_wrapper;
// Sleuthkit::FileSystem struct
static struct tsk4r_fs_wrapper {
    TSK_FS_INFO * filesystem;
}tsk4r_fs_wrapper;

// alloc & dealloc
static void deallocate_image(struct tsk4r_wrapper * ptr){
	//xfree((TSK_IMG_INFO *)image);
	TSK_IMG_INFO *image = ptr->image;
	tsk_img_close(image);
	xfree(ptr);
}

static void deallocate_volume(struct tsk4r_vs_wrapper * ptr){
	TSK_VS_INFO *volume = ptr->volume;
	tsk_vs_close(volume);
	xfree(ptr);
}
static void deallocate_filesystem(struct tsk4r_fs_wrapper * ptr){
    TSK_FS_INFO *filesystem = ptr->filesystem;
    tsk_fs_close(filesystem);
    xfree(ptr);
}

static VALUE allocate_image(VALUE klass){
//	TSK_IMG_INFO *image = ALLOC(TSK_IMG_INFO);
//	TSK_IMG_INFO * image = malloc(sizeof(TSK_IMG_INFO));
//	fprintf(stdout, "allocation complete.\n");
//	return Data_Wrap_Struct(klass, 0, 0, image);

	//struct tsk4r_wrapper * ptr = malloc(sizeof(struct tsk4r_wrapper)); // worked without malloc.  Do I need this?
	struct tsk4r_wrapper * ptr = ALLOC(struct tsk4r_wrapper); // this might be improvement over previous
	return Data_Make_Struct(klass, struct tsk4r_wrapper, 0, deallocate_image, ptr);
	//return Data_Wrap_Struct(klass, 0, deallocate, ptr);
	/*
	TSK_IMG_INFO *sval;
	return Data_Make_Struct(klass, TSK_IMG_INFO, 0, deallocate, sval);
	 */
}
static VALUE allocate_volume(VALUE klass){
	struct tsk4r_vs_wrapper * ptr;
	ptr = ALLOC(struct tsk4r_vs_wrapper);
	return Data_Make_Struct(klass, struct tsk4r_vs_wrapper, 0, deallocate_volume, ptr);
}

static VALUE allocate_filesystem(VALUE klass){
    printf("allocate_filesystem starting...\n");
    struct tsk4r_fs_wrapper * ptr;
    ptr = ALLOC(struct tsk4r_fs_wrapper);
    printf("finishing allocation.\n");
    return Data_Make_Struct(klass, struct tsk4r_fs_wrapper, 0, deallocate_filesystem, ptr);
}

static VALUE image_open(VALUE self, VALUE filename_str) {
	char * filename;
	struct tsk4r_wrapper * ptr;
	Data_Get_Struct(self, struct tsk4r_wrapper, ptr);

	VALUE img_size;
	VALUE img_sector_size;
	fprintf(stdout, "opening %s.\n", StringValuePtr(filename_str));

	Check_Type(filename_str, T_STRING);
	rb_str_modify(filename_str);
	filename=StringValuePtr(filename_str);
	ptr->fn_given = (char *)filename;
	ptr->image = tsk_img_open_sing(filename, TSK_IMG_TYPE_DETECT, 0);
	fprintf(stdout, "attempt to open %s complete\n", (char *)filename);
	if (ptr->image == NULL) {
		rb_warn("unable to open disk.\n");
	}
	TSK_IMG_INFO *image = ptr->image;
	
	img_size = INT2NUM((int)image->size);
	rb_iv_set(self, "@size", img_size);
	img_sector_size = INT2NUM((int)image->sector_size);
	rb_iv_set(self, "@sec_size", img_sector_size);
	rb_iv_set(self, "@type", INT2NUM((int)image->itype));
	
	fprintf(stdout, "opening disk image of %d bytes.\n", (int)image->size ); // dev only
	fprintf(stdout, "disk image has sectors %d bytes in size.\n", (int)image->sector_size ); // dev only

	return self;
}

// init an Image object (was lacking 'static' earlier)
static VALUE initialize_disk_image(int argc, VALUE *args, VALUE self){
	VALUE fn;
//	static struct tsk4r_wrapper * ptr;
	rb_scan_args(argc, args, "01", &fn);
	if( ! NIL_P(fn)) {
		image_open(self, fn);
	} 
	return self;
}

static VALUE initialize_volume(int argc, VALUE *args, VALUE self) {
	VALUE offset; VALUE vs_type;
	VALUE * img_obj;
	rb_scan_args(argc, args, "11", &img_obj);
	if (rb_obj_is_kind_of((VALUE)img_obj, rb_cClass1)) {
		open_volume(self, (VALUE)img_obj);
	} else {
		rb_raise(rb_eTypeError, "Wrong argument type for arg1: (Sleuthkit::Image expected)");
	}
	
	return self;
}

static VALUE initialize_filesystem(int argc, VALUE *args, VALUE self){
    VALUE * img_obj;
    rb_scan_args(argc, args, "1", &img_obj);

    printf("filesystem object init with %d args.\n", argc);
    printf("argument scan complete\n");
    /*
    printf("test1 %d\n", RTEST(rb_respond_to(img_obj, image_open)));
    printf("test2 %d\n", RTEST(rb_respond_to(img_obj, open_volume)));
    printf("test3 %d\n", RTEST(rb_is_instance_id(rb_cClass1)));
    if (RTEST(rb_respond_to(img_obj, image_open))) {
        printf("will open image object passed as arg1\n");
        open_filesystem(self, (VALUE)img_obj);
    }
    if (RTEST(rb_respond_to((VALUE)img_obj, open_volume))) {
        printf("will open volume object passed as arg1\n");
        open_filesystem_from_vol(self, (VALUE)img_obj);
    } else {
        rb_raise(rb_eTypeError, "Wrong argument type for arg1: (Sleuthkit::Image expected)");
    }
     */
    printf("will open image object passed as arg1\n");
    open_filesystem(self, (VALUE)img_obj);
    //rb_iv_set(self, "@root_inum", INT2NUM(1968));
    return self;
}

static VALUE open_volume(VALUE self, VALUE img_obj) {
	TSK_VS_INFO * vs_ptr;
	struct tsk4r_wrapper * rb_image;
//	TSK_VS_PART_INFO * partition_list;
//	partition_list = vs_ptr->part_list;
	Data_Get_Struct(img_obj, struct tsk4r_wrapper, rb_image);
	TSK_IMG_INFO * disk = rb_image->image;

	vs_ptr = tsk_vs_open(disk, 0, TSK_VS_TYPE_DETECT);
	printf("disk has sector size: %d\n", (int)disk->sector_size );
	printf("vs_ptr has partition count: %d\n", (int)vs_ptr->part_count);
	printf("vs_ptr has vs_type: %d\n", (int)vs_ptr->vstype);
	printf("vs_ptr type description: %s\n", (char *)tsk_vs_type_todesc(vs_ptr->vstype));
	printf("vs_ptr has offset: %d\n", (int)vs_ptr->offset);
	printf("vs_ptr has block size: %d\n", (int)vs_ptr->block_size);
	printf("vs_ptr has endian: %d\n", (int)vs_ptr->endian);
	
	rb_iv_set(self, "@partition_count", INT2NUM((int)vs_ptr->part_count));
	rb_iv_set(self, "@endian", INT2NUM((int)vs_ptr->endian));
	rb_iv_set(self, "@offset", INT2NUM((int)vs_ptr->offset));
	rb_iv_set(self, "@block_size", INT2NUM((int)vs_ptr->block_size));
	
//	printf("vs_part_ptr has desc: %s\n", (char *)partition_list->desc);
//	printf("vs_part_ptr has start: %d\n", (int)partition_list->start);
//	printf("vs_part_ptr has length: %d\n", (int)partition_list->len);
//	fprintf(stdout, "I will open a volume, eventually.\n");
    return self;
}

static VALUE open_filesystem(VALUE self, VALUE img_obj) {
    printf("open_filesystem here....\n");
    struct tsk4r_fs_wrapper * fs_ptr;
    Data_Get_Struct(self, struct tsk4r_fs_wrapper, fs_ptr);
    struct tsk4r_wrapper * rb_image;
    Data_Get_Struct(img_obj, struct tsk4r_wrapper, rb_image);
    TSK_IMG_INFO * disk = rb_image->image;
    fs_ptr->filesystem = tsk_fs_open_img(disk, 0, TSK_FS_TYPE_DETECT);
    //rb_iv_set(self, "@root_inum", INT2NUM((int)fs_ptr->filesystem->root_inum));
    TSK_INUM_T my_root_inum = 1968;
    TSK_INUM_T my_last_inum = 43;
    unsigned int my_block_size = 0;
    if (fs_ptr->filesystem != NULL) {
        my_root_inum = fs_ptr->filesystem->root_inum; 
        my_last_inum = fs_ptr->filesystem->last_inum;
        my_block_size = fs_ptr->filesystem->block_size;
    } else {
        my_root_inum = 222;
        my_last_inum = 333;
    }
    
    rb_iv_set(self, "@root_inum", INT2NUM(my_root_inum));
    rb_iv_set(self, "@last_inum", INT2NUM(my_last_inum));
    rb_iv_set(self, "@block_size", INT2NUM(my_block_size));

    return self;
}

static VALUE open_filesystem_from_vol(VALUE self, VALUE vol_obj) {
    printf("open filesystem_from_vol here...\n");
    struct tsk4r_fs_wrapper * fs_ptr;
    Data_Get_Struct(self, struct tsk4r_fs_wrapper, fs_ptr);
    struct tsk4r_vs_wrapper * rb_volume;
    Data_Get_Struct(vol_obj, struct tsk4r_vs_wrapper, rb_volume);
    TSK_VS_INFO * volume = rb_volume->volume;
    TSK_VS_PART_INFO * partition1 = rb_volume->volume->part_list;
    fs_ptr->filesystem = tsk_fs_open_vol(partition1, TSK_FS_TYPE_DETECT);
    TSK_INUM_T my_root_inum = 1976;
    if (fs_ptr->filesystem != NULL) {
        my_root_inum = fs_ptr->filesystem->root_inum;
    } else {
        my_root_inum = 444;
    }
    rb_iv_set(self, "@root_inum", INT2NUM(my_root_inum));
    return self;
}

static VALUE close_volume(VALUE self){
	return Qnil;
}

static VALUE read_volume_block(int argc, VALUE *args, VALUE self){
	return Qnil;
}

static VALUE walk_volume(VALUE self){
	return Qnil;
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
	struct tsk4r_wrapper * new_ptr;	
	Data_Get_Struct(self, struct tsk4r_wrapper, new_ptr);

	TSK_IMG_INFO * image = new_ptr->image;
	char * orig_fn = new_ptr->fn_given;
//	fprintf(stdout, "struct stored filename: >%s<\n", orig_fn);
	VALUE s_size;
	s_size = INT2FIX(22);
	unsigned int sss = image->sector_size;
	s_size = INT2FIX(sss);
//	fprintf(stdout, "disk size: %d\n", (int)image->size);
//	fprintf(stdout, "sector size: %d\n", sss);
	FIXNUM_P(s_size);
	return s_size;
}

static VALUE image_type(VALUE self){
	TSK_IMG_INFO *image;
	Data_Get_Struct(self, TSK_IMG_INFO, image);
	return INT2NUM((int)image->itype);
}

// The initialization method for this module
void Init_tsk4r() {
	rb_mtsk4r = rb_define_module("Sleuthkit");
	
	// class definitions
	rb_cClass1 = rb_define_class_under(rb_mtsk4r, "Image", rb_cObject);
	rb_cClass2 = rb_define_class_under(rb_mtsk4r, "Volume", rb_cObject);
	rb_cClass3 = rb_define_class_under(rb_mtsk4r, "FileSystem", rb_cObject);
	
	// allocation functions
	rb_define_alloc_func(rb_cClass1, allocate_image);
	rb_define_alloc_func(rb_cClass2, allocate_volume);
    rb_define_alloc_func(rb_cClass3, allocate_filesystem);


	// sub classes
	//rb_cClass3 = rb_define_class_under(rb_cClass2, "ThirdClass", rb_cObject);

	
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
	rb_define_method(rb_cClass1, "initialize", initialize_disk_image, -1);
	rb_define_method(rb_cClass1, "image_open", image_open, 1);
	rb_define_method(rb_cClass1, "image_size", image_size, 0);
	rb_define_method(rb_cClass1, "sector_size", sector_size, 0);
	rb_define_method(rb_cClass1, "image_type", image_type, 0);

	// attributes (read only)
	rb_define_attr(rb_cClass1, "size", 1, 0);
	rb_define_attr(rb_cClass1, "sec_size", 1, 0);
	rb_define_attr(rb_cClass1, "type", 1, 0);

	
	/* Sleuthkit::Volume */
	// object methods for Volume objects
	rb_define_method(rb_cClass2, "initialize", initialize_volume, -1);
	rb_define_method(rb_cClass2, "open", open_volume, 1); // change arg1 to klass?
	rb_define_method(rb_cClass2, "close", close_volume, 1);
	rb_define_method(rb_cClass2, "read_block", read_volume_block, 3); //read block given start and no. of blocks
	rb_define_method(rb_cClass2, "walk", walk_volume, 1);
	
	// attributes
	rb_define_attr(rb_cClass2, "partition_count", 1, 0);
	rb_define_attr(rb_cClass2, "endian", 1, 0);
	rb_define_attr(rb_cClass2, "offset", 1, 0);
	rb_define_attr(rb_cClass2, "block_size", 1, 0);
    
    /* Sleuthkit::FileSystem */
    // object methods for FileSystem objects
    rb_define_method(rb_cClass3, "initialize", initialize_filesystem, -1);
    rb_define_method(rb_cClass3, "open", open_filesystem, 1); // change arg1 to klass?

    
    // attributes
    rb_define_attr(rb_cClass3, "root_inum", 1, 0);
    rb_define_attr(rb_cClass3, "last_inum", 1, 0);
    rb_define_attr(rb_cClass3, "block_size", 1, 0);
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


