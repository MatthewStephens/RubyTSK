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
extern VALUE rb_cTSKVolumeSystem;
extern VALUE rb_cTSKVolumePart;

struct tsk4r_img {
  TSK_IMG_INFO * image;
  char * fn_given;
};

// Sleuthkit::VolumeSystem functions

VALUE allocate_volume_system(VALUE klass){
  struct tsk4r_vs_wrapper * ptr;
  return Data_Make_Struct(klass, struct tsk4r_vs_wrapper, 0, deallocate_volume_system, ptr);
}

void deallocate_volume_system(struct tsk4r_vs_wrapper * ptr){
  TSK_VS_INFO *volume = ptr->volume;
  tsk_vs_close(volume);
  xfree(ptr);
}

VALUE initialize_volume_system(int argc, VALUE *args, VALUE self) {

  VALUE * img_obj;
  rb_scan_args(argc, args, "11", &img_obj);
  if (rb_obj_is_kind_of((VALUE)img_obj, rb_cTSKImage)) {
    open_volume_system(self, (VALUE)img_obj);
  } else {
    rb_raise(rb_eTypeError, "Wrong argument type for arg1: (Sleuthkit::Image expected)");
  }
  
  return self;
}


VALUE open_volume_system(VALUE self, VALUE img_obj) {

  struct tsk4r_vs_wrapper * vs_ptr;
  Data_Get_Struct(self, struct tsk4r_vs_wrapper, vs_ptr);

  // open disk image
  struct tsk4r_img * rb_image;
  Data_Get_Struct(img_obj, struct tsk4r_img, rb_image);
  TSK_IMG_INFO * disk = rb_image->image;

  // open volume system and assign to data pointer
  vs_ptr->volume = tsk_vs_open(disk, 0, TSK_VS_TYPE_DETECT);
  TSK_VS_INFO * volume_system = vs_ptr->volume;


//  printf("disk has sector size: %d\n", (int)disk->sector_size );
  printf("N.B. vs_ptr has partition count: %d\n", (int)volume_system->part_count);
  printf("vs_ptr has vs_type: %d\n", (int)volume_system->vstype);
  printf("vs_ptr type description: %s\n", (char *)tsk_vs_type_todesc(volume_system->vstype));
  printf("vs_ptr has offset: %d\n", (int)volume_system->offset);
  printf("vs_ptr has block size: %d\n", (int)volume_system->block_size);
  printf("vs_ptr has endian: %d\n", (int)volume_system->endian);
  
  rb_iv_set(self, "@partition_count", INT2NUM((int)volume_system->part_count));
  rb_iv_set(self, "@volume_system_type", INT2NUM((int)volume_system->vstype));
  rb_iv_set(self, "@description", rb_str_new2( (char *)tsk_vs_type_todesc(volume_system->vstype) ));
  rb_iv_set(self, "@endian", INT2NUM((int)volume_system->endian));
  rb_iv_set(self, "@offset", INT2NUM((int)volume_system->offset));
  rb_iv_set(self, "@block_size", INT2NUM((int)volume_system->block_size));
  
  return self;
}

// called on VolumeSystem, returns first VolumePart
VALUE volume_expose_part(VALUE self) {
  VALUE volume_part;
  
  // call VolumePart.new, passing self
  volume_part = rb_funcall(rb_cTSKVolumePart, rb_intern("new"), 1, self);

  return volume_part;
}

VALUE volume_system_iterate(int argc, VALUE *args, VALUE self) {
  VALUE block = Qnil;
  rb_scan_args(argc, args, "0&", &block);

  if(RTEST(block)) {
    long i = 0; VALUE part_count = rb_attr_get(self, rb_intern("@partition_count"));
    while (i < FIX2LONG(part_count)) {
      printf("LOOPING UNTIL %ld (at %ld currently)\n", FIX2LONG(part_count), i);
      printf("creating vpart object number: %ld\n", i);
      VALUE vpart = rb_funcall(rb_cTSKVolumePart, rb_intern("new"), 2, self, LONG2NUM(i));
      rb_funcall(block, rb_intern("call"), 1, vpart);
      i++;
    }
  } else {
    rb_raise(rb_eArgError, "a block is required");
  }
  
  return Qnil;

}

VALUE volume_expose_part_by_idx(VALUE self, VALUE index) {
  VALUE volume_part;
  
  // call VolumePart.new, passing self
  volume_part = rb_funcall(rb_cTSKVolumePart, rb_intern("new"), 2, self, index);
  
  return volume_part;
}

// Sleuthkit::VolumePart functions

VALUE open_next_volume_part(VALUE self) {
  
  VALUE next_part;
  struct tsk4r_vpart_wrapper * mydata;
  Data_Get_Struct(self, struct tsk4r_vpart_wrapper, mydata);
  next_part = Qnil;
  if (mydata->volume_part->next != NULL) {

  }
  
  return next_part;
}

VALUE open_volume_part(int argc, VALUE *args, VALUE self){
  // self, vs_obj, index, e.g.
  VALUE * vs_obj; VALUE index;
  rb_scan_args(argc, args, "12", &vs_obj, &index);
  printf("open_volume_part (aka #open) called\n");
  if (NIL_P(index)) { index = INT2FIX(0); }
  printf("open_volume_part recd index=%ld\n", FIX2INT(index));
  struct tsk4r_vpart_wrapper * partition;
  const TSK_VS_PART_INFO * vp_ptr;
  struct tsk4r_vs_wrapper * parent;  // pointer to parent volume system
  
  if ( ! rb_obj_is_kind_of((VALUE)vs_obj, rb_cTSKVolumeSystem)) {
    rb_raise(rb_eTypeError, "Arg1 is NOT Sleuthkit::VolumeSystem\n");
  }
  
  // open volume system
  Data_Get_Struct(vs_obj, struct tsk4r_vs_wrapper, parent);
  printf("get struct from vs returned\n");
  printf("volume struct size is %ld\n", sizeof(parent->volume));
  printf("volume struct pointer size is %ld\n", sizeof(parent));
  
  // open self's struct and assign partition to it
  Data_Get_Struct(self, struct tsk4r_vpart_wrapper, partition);

  printf("vs_ptr count is: %d\n", parent->volume->part_count); //no. of partitions
  TSK_VS_INFO * volume_system = parent->volume;
  printf("volume_system assigned to vs_ptr->volume\n");

  TSK_PNUM_T idx = FIX2LONG(index);
  vp_ptr = tsk_vs_part_get(volume_system, idx);
  partition->volume_part = vp_ptr;
  printf("vp_ptr assigned to return of tsk_vs_part_get\n");
  printf("table number: %d\n", vp_ptr->table_num);

  rb_iv_set(self, "@start", INT2NUM((int)vp_ptr->start));
  rb_iv_set(self, "@length", INT2NUM((int)vp_ptr->len));
  rb_iv_set(self, "@description", rb_str_new2( (char *)vp_ptr->desc));
  rb_iv_set(self, "@table_number", INT2NUM((int)vp_ptr->table_num));
  rb_iv_set(self, "@slot_number", INT2NUM((int)vp_ptr->slot_num));
  rb_iv_set(self, "@address", INT2NUM((int)vp_ptr->addr));
  rb_iv_set(self, "@flags", INT2NUM((int)vp_ptr->flags));

  
  return self;
}

VALUE allocate_volume_part(VALUE klass){
  struct tsk4r_vpart_wrapper * ptr;
  return Data_Make_Struct(klass, struct tsk4r_vpart_wrapper, 0, deallocate_volume_part, ptr);
}

// this deallocate might need further work
void deallocate_volume_part(struct tsk4r_vpart_wrapper * ptr){
  xfree(ptr);
}

VALUE initialize_volume_part(int argc, VALUE *args, VALUE self){
  VALUE * vs_obj; VALUE index;
  fprintf(stdout, "initialize_volume_part called.\n");
  rb_scan_args(argc, args, "11", &vs_obj, &index);
  if (NIL_P(index)) { index = INT2FIX(0); }
  printf("initialize_volume_part recd index=%ld\n", FIX2INT(index));
  if (rb_obj_is_kind_of((VALUE)vs_obj, rb_cTSKVolumeSystem)) {
    printf("trying to call open_volume_part passing VolumeSystem\n");
    rb_funcall(self, rb_intern("open"), 2, (VALUE)vs_obj, (VALUE)index);
//    open_volume_part(self, (VALUE)vs_obj, ));
//    open_volume_part(3, [self, (VALUE)vs_obj, (VALUE)index], self);
  } else if (rb_obj_is_kind_of((VALUE)vs_obj, rb_cTSKVolumePart)) {
    printf("trying to call open_volume_part passing VolumePart\n");
    rb_funcall(self, rb_intern("open"), 2, (VALUE)vs_obj, (VALUE)index);
  }
  else {
    rb_raise(rb_eTypeError, "Wrong argument type for arg1: (Sleuthkit::VolumeSystem expected)");
  }
  
  return self;
}

VALUE read_volume_part_block(int argc, VALUE *args, VALUE self) {
  printf("vs_part: method not built yet!\n");
  return self;
}
