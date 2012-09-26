//
//  image.c
//  RubyTSK
//
//  Created by ms3uf on 9/13/12.
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

  }
  else if (rb_obj_is_kind_of(filename_location, rb_cArray)) {
    long i;
    const TSK_TCHAR * images[RARRAY(filename_location)->len];
    for (i=0; i < RARRAY(filename_location)->len; i++) {
      VALUE rstring = rb_ary_entry(filename_location, i);
      images[i] = StringValuePtr(rstring);
    }
    int count = (int)RARRAY(filename_location)->len;
    long len = i;
    printf("count: %d, image->len %ld\n", count, len );

    ptr->image = tsk_img_open(count, images, (TSK_IMG_TYPE_ENUM)dtype, 0); // 0=default sector size
    
  }
  else {
    rb_raise(rb_eArgError, "Arg1 should be String or Array of strings.");
  }

  if (ptr->image == NULL) {
    rb_warn("unable to open disk.\n");
  }
  
  TSK_IMG_INFO *image = ptr->image;
  if (ptr->image != NULL) {
    img_size = LONG2NUM(image->size);
    img_sector_size = INT2NUM((int)image->sector_size);
    TSK_IMG_TYPE_ENUM typenum = image->itype;
    description = image_type_to_desc(typenum);
    name = image_type_to_name(typenum);

    rb_iv_set(self, "@size", img_size);
    rb_iv_set(self, "@sec_size", img_sector_size);
    rb_iv_set(self, "@type", INT2NUM((int)typenum));
    rb_iv_set(self, "@description", description);
    rb_iv_set(self, "@name", name);
    
    return Qtrue;
  } else {
    printf("problem opening disk\n");
    return Qnil;
  }
}

// init an Image object, passing params to image_open
// note that class of arg1, if array, will override requests for 'single' image
VALUE initialize_disk_image(int argc, VALUE *args, VALUE self){
  VALUE filename; VALUE flag; VALUE result;

  //  static struct tsk4r_img_wrapper * ptr;
  rb_scan_args(argc, args, "12", &filename, &flag);
  if (NIL_P(flag)) { flag = INT2NUM(0); }

  if ( ! NIL_P(filename) && ! NIL_P(flag) ) {
    printf("disk_type set.\n");

  } else if ( ! NIL_P(filename)) {
    printf("No disk_type requested; defaulting to TSK_IMG_TYPE_DETECT\n");
    flag = (TSK_IMG_TYPE_ENUM)"TSK_IMG_TYPE_DETECT";
  } else {
    rb_raise (rb_eRuntimeError, "invalid arguments");

  }
  
  if( ! NIL_P(filename)) {
    // string for single image, array for split images
    rb_iv_set(self, "@path", filename);
    if (rb_obj_is_kind_of(filename, rb_cString)) {
      printf("opening single image\n");
      result = image_open(self, filename, flag); // passing flag (disk_type) as ruby FIXNUM
    } else if (rb_obj_is_kind_of(filename, rb_cArray)) {
      printf("opening split image (flag=%ld)\n", FIX2LONG(flag));
      result = image_open(self, filename, flag);
    } else {
      rb_raise(rb_eTypeError, "arg1 must be String or Array");
    }
  } else {
    rb_raise(rb_eArgError, "Arg1 must be filename (string)");
  }
  if (RTEST(result)) {
    return self;
  } else {
    return Qnil;
  }
}


VALUE sector_size(VALUE self){
  struct tsk4r_img_wrapper * new_ptr;
  Data_Get_Struct(self, struct tsk4r_img_wrapper, new_ptr);
  
  TSK_IMG_INFO * image = new_ptr->image;
  //  char * orig_fn = new_ptr->fn_given;
  //  fprintf(stdout, "struct stored filename: >%s<\n", orig_fn);
  VALUE s_size;
  if (image != NULL) {
    unsigned int sss = image->sector_size;
    s_size = INT2FIX(sss);
  } else {
    s_size = INT2FIX(22);
  }
  //  fprintf(stdout, "disk size: %d\n", (int)image->size);
  //  fprintf(stdout, "sector size: %d\n", sss);
  //  FIXNUM_P(s_size);
  return s_size;
}

VALUE image_size(VALUE self){
  TSK_IMG_INFO* image;
  fprintf(stdout, "DATA_PTR(self): %lu\n", (long)DATA_PTR(self));
  
  Data_Get_Struct(self, TSK_IMG_INFO, image);
  fprintf(stdout, "DATA_PTR(self): %lu\n", (long)DATA_PTR(self));
  
  //fprintf(stdout, "image size: %d\n", (int)image->size);
  return INT2NUM((int)image->size);
}

VALUE image_type(VALUE self){
  TSK_IMG_INFO *image;
  Data_Get_Struct(self, TSK_IMG_INFO, image);
  return INT2NUM((int)image->itype);
}

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
      printf("string is %s\n", StringValuePtr(rb_obj));
      char *str = StringValuePtr(rb_obj);
      //TO DO: convert string to value of Sleuthkit::VolumeSystem::TSK_VS_TYPE_ENUM[string.to_sym]
      printf("flag is %s\n", str);
      flag = (TSK_IMG_TYPE_ENUM *)0;
      break;
      
    case T_FIXNUM:
      printf("disk_type is %ld\n", NUM2INT(rb_obj));
      long num = NUM2INT(rb_obj);
      flag = (TSK_IMG_TYPE_ENUM *)num;
      printf("flag is %ld\n", num);
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