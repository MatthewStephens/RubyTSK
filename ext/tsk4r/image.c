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

VALUE image_open(VALUE self, VALUE filename_str) {
  char * filename;
  struct tsk4r_img_wrapper * ptr;
  Data_Get_Struct(self, struct tsk4r_img_wrapper, ptr);
  
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
  if (ptr->image != NULL) {
    img_size = INT2NUM((int)image->size);
    rb_iv_set(self, "@size", img_size);
    img_sector_size = INT2NUM((int)image->sector_size);
    rb_iv_set(self, "@sec_size", img_sector_size);
    rb_iv_set(self, "@type", INT2NUM((int)image->itype));
    
    fprintf(stdout, "opening disk image of %d bytes.\n", (int)image->size ); // dev only
    fprintf(stdout, "disk image has sectors %d bytes in size.\n", (int)image->sector_size ); // dev only
  }
  return self;
}

// init an Image object (was lacking 'static' earlier)
VALUE initialize_disk_image(int argc, VALUE *args, VALUE self){
  VALUE fn;
  //  static struct tsk4r_img_wrapper * ptr;
  rb_scan_args(argc, args, "01", &fn);
  if( ! NIL_P(fn)) {
    image_open(self, fn);
  } 
  return self;
}