//
//  image.c
//  RubyTSK
//
//  Created by Matthew Stephens on 9/13/12.
//
//

#include <stdio.h>
#include <ruby.h>
#include "image.h"

// prototypes (private)
TSK_IMG_TYPE_ENUM * get_img_flag();

// functions
VALUE allocate_image(VALUE klass){
  struct tsk4r_img_wrapper * ptr;
  return Data_Make_Struct(klass, struct tsk4r_img_wrapper, 0, deallocate_image, ptr);
}

void deallocate_image(struct tsk4r_img_wrapper * ptr){
  TSK_IMG_INFO *image = ptr->image;
  tsk_img_close(image);
  xfree(ptr);
}

VALUE image_open(VALUE self, VALUE filename_location, VALUE disk_type_flag) {
  char * filename; int dtype;
  struct tsk4r_img_wrapper * ptr;
  Data_Get_Struct(self, struct tsk4r_img_wrapper, ptr);
  
  VALUE img_size;
  VALUE img_sector_size;
  VALUE description = Qnil; VALUE name = Qnil;
  dtype = FIX2ULONG(disk_type_flag);
  TSK_IMG_TYPE_ENUM * type_flag_num = get_img_flag(disk_type_flag);
  
  if (rb_obj_is_kind_of(filename_location, rb_cString)) {
    fprintf(stdout, "opening %s. (flag=%d)\n", StringValuePtr(filename_location), dtype);
    rb_str_modify(filename_location);
    filename=StringValuePtr(filename_location);
    ptr->image = tsk_img_open_sing(filename, (TSK_IMG_TYPE_ENUM)type_flag_num, 0); // 0=default sector size
    if (ptr->image == NULL) rb_warn("unable to open image %s.\n", StringValuePtr(filename_location));

  }
  else if (rb_obj_is_kind_of(filename_location, rb_cArray)) {
    long i;
    typedef TSK_TCHAR * split_list;
    split_list images[255]; // to do: make array length reflect list's length

    for (i=0; i < RARRAY_LEN(filename_location); i++) {
      VALUE rstring = rb_ary_entry(filename_location, i);
      images[i] = StringValuePtr(rstring);
    }
    int count = (int)RARRAY_LEN(filename_location);

    ptr->image = tsk_img_open(count, (const TSK_TCHAR **)images, (TSK_IMG_TYPE_ENUM)type_flag_num, 0); // 0=default sector size
    VALUE arr_to_s = rb_funcall(filename_location, rb_intern("to_s"), 0, NULL);
    if (ptr->image == NULL) rb_warn("unable to open images %s.\n", StringValuePtr(arr_to_s));

  }
  else {
    rb_raise(rb_eArgError, "Arg1 should be String or Array of strings.");
  }

  if (ptr->image == NULL) {
    rb_funcall(self, rb_intern("taint"), 0, NULL);

    return Qnil;
    
  } else {
    TSK_IMG_INFO *image = ptr->image;

    img_size = LONG2NUM(image->size);
    img_sector_size = INT2NUM((int)image->sector_size);
    TSK_IMG_TYPE_ENUM typenum = image->itype;
    description = image_type_to_desc(typenum);
    name = image_type_to_name(typenum);

    rb_iv_set(self, "@size", img_size);
    rb_iv_set(self, "@sector_size", img_sector_size);
    rb_iv_set(self, "@type", INT2NUM((int)typenum));
    rb_iv_set(self, "@description", description);
    rb_iv_set(self, "@name", name);
    
    return self;
  }
}

// init an Image object, passing params to image_open
// note that class of arg1, if array, will override requests for 'single' image
VALUE initialize_disk_image(int argc, VALUE *args, VALUE self){
  VALUE filename; VALUE rest; VALUE parsed_opts; VALUE flag;
  struct tsk4r_img_wrapper * ptr;

  rb_scan_args(argc, args, "11", &filename, &rest);
  if (NIL_P(rest)) rest = rb_hash_new();
  
  parsed_opts = rb_funcall(self, rb_intern("parse_opts"), 1, rest);

  flag = rb_hash_aref(parsed_opts, ID2SYM(rb_intern("type_flag")));
  if (! rb_obj_is_kind_of(flag, rb_cFixnum)) { flag = INT2NUM(0); }
  
  TSK_IMG_TYPE_ENUM * flag_num = get_img_flag(flag);
  if ( (! NIL_P(filename)) && (flag_num > 0)  ) {
    rb_iv_set(self, "@auto_detect", Qfalse);

  } else if ( ! NIL_P(filename)) {
    flag = INT2FIX((TSK_IMG_TYPE_ENUM)0); // auto-detect
    rb_iv_set(self, "@auto_detect", Qtrue);

  } else {
    rb_raise (rb_eRuntimeError, "invalid arguments");

  }

  if( ! NIL_P(filename)) {
    // string for single image, array for split images
    rb_iv_set(self, "@path", filename);
    if (rb_obj_is_kind_of(filename, rb_cString)) {
      image_open(self, filename, flag); // passing flag (disk_type) as ruby FIXNUM
    } else if (rb_obj_is_kind_of(filename, rb_cArray)) {
      image_open(self, filename, flag);
    } else {
      rb_raise(rb_eTypeError, "arg1 must be String or Array");
    }
  } else {
    rb_raise(rb_eArgError, "Arg1 must be filename (string)");
  }
  
  Data_Get_Struct(self, struct tsk4r_img_wrapper, ptr);
  if ( ptr->image != NULL ) {
    return self;
  } else {
    return Qnil;
  }
}

// helper methods
VALUE image_type_to_desc(TSK_IMG_TYPE_ENUM num) {
  const char * description;
  description = tsk_img_type_todesc(num);
  return rb_str_new2(description);
}

VALUE image_type_to_name(TSK_IMG_TYPE_ENUM num) {
  const char * name;
  name = tsk_img_type_toname(num);
  return rb_str_new2(name);
}

// helper method to convert ruby integers to TSK_IMG_TYPE_ENUM values
TSK_IMG_TYPE_ENUM * get_img_flag(VALUE rb_obj) {
  TSK_IMG_TYPE_ENUM * flag;
  switch (TYPE(rb_obj)) {
    case T_STRING:
      //TO DO: convert string to value of Sleuthkit::VolumeSystem::TSK_VS_TYPE_ENUM[string.to_sym]
      flag = (TSK_IMG_TYPE_ENUM *)0;
      break;
      
    case T_FIXNUM:
      flag = (TSK_IMG_TYPE_ENUM *)NUM2INT(rb_obj);
      break;
      
    case T_SYMBOL:
      // TO DO
      flag = (TSK_IMG_TYPE_ENUM *)0;
      break;
      
    default:
      flag = (TSK_IMG_TYPE_ENUM *)0;
      break;
  }
  return flag;
}