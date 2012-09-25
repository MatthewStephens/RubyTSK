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

VALUE allocate_image(VALUE klass){
  //  TSK_IMG_INFO *image = ALLOC(TSK_IMG_INFO);
  //  TSK_IMG_INFO * image = malloc(sizeof(TSK_IMG_INFO));
  //  fprintf(stdout, "allocation complete.\n");
  //  return Data_Wrap_Struct(klass, 0, 0, image);
  
  //struct tsk4r_img_wrapper * ptr = malloc(sizeof(struct tsk4r_img_wrapper)); // worked without malloc.  Do I need this?
  struct tsk4r_img_wrapper * ptr;
  //ptr = ALLOC(struct tsk4r_img_wrapper); // this might be improvement over previous
  return Data_Make_Struct(klass, struct tsk4r_img_wrapper, 0, deallocate_image, ptr);
  //return Data_Wrap_Struct(klass, 0, deallocate, ptr);
  /*
   TSK_IMG_INFO *sval;
   return Data_Make_Struct(klass, TSK_IMG_INFO, 0, deallocate, sval);
   */
}

void deallocate_image(struct tsk4r_img_wrapper * ptr){
  //xfree((TSK_IMG_INFO *)image);
  TSK_IMG_INFO *image = ptr->image;
  tsk_img_close(image);
  xfree(ptr);
}

VALUE image_open(VALUE self, VALUE filename_str, VALUE disk_type) {
  char * filename; int dtype;
  struct tsk4r_img_wrapper * ptr;
  Data_Get_Struct(self, struct tsk4r_img_wrapper, ptr);
  
  VALUE img_size;
  VALUE img_sector_size;
  VALUE description = Qnil; VALUE name = Qnil;
  dtype = FIX2ULONG(disk_type);
  fprintf(stdout, "opening %s. (flag=%d)\n", StringValuePtr(filename_str), dtype);
  
  Check_Type(filename_str, T_STRING);
  rb_str_modify(filename_str);
  filename=StringValuePtr(filename_str);

  ptr->fn_given = (char *)filename;
  ptr->image = tsk_img_open_sing(filename, (TSK_IMG_TYPE_ENUM)dtype, 0);
  fprintf(stdout, "attempt to open %s complete\n", (char *)filename);
  if (ptr->image == NULL) {
    rb_warn("unable to open disk.\n");
  }
  TSK_IMG_INFO *image = ptr->image;
  if (ptr->image != NULL) {
    img_size = INT2NUM((int)image->size);
    rb_iv_set(self, "@size", img_size);
    img_sector_size = INT2NUM((int)image->sector_size);
    rb_iv_set(self, "@sec_size", img_sector_size);
    TSK_IMG_TYPE_ENUM typenum = image->itype;
    rb_iv_set(self, "@type", INT2NUM((int)typenum));
    description = image_type_to_desc(typenum);
    rb_iv_set(self, "@description", description);
    name = image_type_to_name(typenum);
    rb_iv_set(self, "@name", name);
    
    fprintf(stdout, "opening disk image of %d bytes.\n", (int)image->size ); // dev only
    fprintf(stdout, "disk image has sectors %d bytes in size.\n", (int)image->sector_size ); // dev only
    return Qtrue;
  } else {
    return Qfalse;
  }
//  return self;
}

// init an Image object, passing params to image_open
VALUE initialize_disk_image(int argc, VALUE *args, VALUE self){
  VALUE filename; VALUE disk_type; VALUE disk_type_num = INT2NUM(0);
  TSK_IMG_TYPE_ENUM flag;
  //  static struct tsk4r_img_wrapper * ptr;
  rb_scan_args(argc, args, "12", &filename, &disk_type);
  
  if ( ! NIL_P(filename) && ! NIL_P(disk_type) ) {
    printf("disk_type set.\n");
    switch (TYPE(disk_type)) {
      case T_STRING:
        printf("string is %s\n", StringValuePtr(disk_type));
        flag = *StringValuePtr(disk_type);
        //TO DO: convert string to value of Sleuthkit::Image::TSK_IMG_TYPE_ENUM[string.to_sym]
        printf("flag is %d\n", flag);
        break;
        
      case T_FIXNUM:
        printf("disk_type is %ld\n", NUM2INT(disk_type));
        flag = NUM2DBL(disk_type);
        disk_type_num = disk_type;
        printf("flag is %d\n", flag);
        break;
        
      case T_SYMBOL:
        // TO DO
        flag = 0;
        break;
        
      default:
        flag = 0;
        break;
    }

  } else if ( ! NIL_P(filename)) {
    printf("No disk_type requested; defaulting to TSK_IMG_TYPE_DETECT\n");
    disk_type = (TSK_IMG_TYPE_ENUM)"TSK_IMG_TYPE_DETECT";
    flag = TSK_IMG_TYPE_DETECT;
  } else {
    rb_raise (rb_eRuntimeError, "invalid arguments");

  }
  
  fprintf(stdout, "initializing with filename: %s\n", StringValuePtr(filename));
  fprintf(stdout, "initializing with flag: %d\n", flag);
  if( ! NIL_P(filename)) {
    image_open(self, filename, disk_type_num); // passing flag (disk_type) as ruby FIXNUM
  } else {
    rb_raise(rb_eArgError, "Arg1 must be filename (string)");
  }
  return self;
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