//
//  volume.c
//  RubyTSK
//
//  Created by ms3uf on 9/13/12.
//
//

#include <stdio.h>
#include <ruby.h>
#include "volume.h"

extern VALUE rb_cTSKImage;

struct tsk4r_img {
  TSK_IMG_INFO * image;
  char * fn_given;
};

// Sleuthkit::Volume functions

VALUE allocate_volume(VALUE klass){
  struct tsk4r_vs_wrapper * ptr;
  //  ptr = ALLOC(struct tsk4r_vs_wrapper);
  return Data_Make_Struct(klass, struct tsk4r_vs_wrapper, 0, deallocate_volume, ptr);
}

void deallocate_volume(struct tsk4r_vs_wrapper * ptr){
  TSK_VS_INFO *volume = ptr->volume;
  tsk_vs_close(volume);
  xfree(ptr);
}

VALUE initialize_volume(int argc, VALUE *args, VALUE self) {
  //  VALUE offset;
  //  VALUE vs_type;
  VALUE * img_obj;
  rb_scan_args(argc, args, "11", &img_obj);
  if (rb_obj_is_kind_of((VALUE)img_obj, rb_cTSKImage)) {
    open_volume(self, (VALUE)img_obj);
  } else {
    rb_raise(rb_eTypeError, "Wrong argument type for arg1: (Sleuthkit::Image expected)");
  }
  
  return self;
}

VALUE open_volume(VALUE self, VALUE img_obj) {
  TSK_VS_INFO * vs_ptr;
  struct tsk4r_img * rb_image;
  //  TSK_VS_PART_INFO * partition_list;
  //  partition_list = vs_ptr->part_list;
  Data_Get_Struct(img_obj, struct tsk4r_img, rb_image);
  TSK_IMG_INFO * disk = rb_image->image;
  
  vs_ptr = tsk_vs_open(disk, 0, TSK_VS_TYPE_DETECT);
//  printf("disk has sector size: %d\n", (int)disk->sector_size );
//  printf("vs_ptr has partition count: %d\n", (int)vs_ptr->part_count);
//  printf("vs_ptr has vs_type: %d\n", (int)vs_ptr->vstype);
//  printf("vs_ptr type description: %s\n", (char *)tsk_vs_type_todesc(vs_ptr->vstype));
//  printf("vs_ptr has offset: %d\n", (int)vs_ptr->offset);
//  printf("vs_ptr has block size: %d\n", (int)vs_ptr->block_size);
//  printf("vs_ptr has endian: %d\n", (int)vs_ptr->endian);
  
  rb_iv_set(self, "@partition_count", INT2NUM((int)vs_ptr->part_count));
  rb_iv_set(self, "@volume_system_type", INT2NUM((int)vs_ptr->vstype));
  rb_iv_set(self, "@description", rb_str_new2( (char *)tsk_vs_type_todesc(vs_ptr->vstype) ));
  rb_iv_set(self, "@endian", INT2NUM((int)vs_ptr->endian));
  rb_iv_set(self, "@offset", INT2NUM((int)vs_ptr->offset));
  rb_iv_set(self, "@block_size", INT2NUM((int)vs_ptr->block_size));
  
  //  printf("vs_part_ptr has desc: %s\n", (char *)partition_list->desc);
  //  printf("vs_part_ptr has start: %d\n", (int)partition_list->start);
  //  printf("vs_part_ptr has length: %d\n", (int)partition_list->len);
  //  fprintf(stdout, "I will open a volume, eventually.\n");
  return self;
}