/*
 *  volume.c: Volume module for tsk4r
 *
 *  tsk4r: The SleuthKit 4 Ruby
 *
 *  Created by Matthew H. Stephens
 *  Copyright 2011,2012 Matthew H. Stephens. All rights reserved.
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
 * Binding to the SleuthKit (libtsk3) library copyright 2003,2012 by Brian Carrier
 */

#include <stdio.h>
#include <string.h>
#include <ruby.h>
#include "volume.h"

extern VALUE rb_cTSKImage;
extern VALUE rb_cTSKVolumeSystem;
extern VALUE rb_cTSKVolumePart;

struct tsk4r_img {
  TSK_IMG_INFO * image;
  char * fn_given;
};

TSK_VS_TYPE_ENUM * get_vs_flag();

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

  VALUE img_obj; VALUE opts;

  rb_scan_args(argc, args, "11", &img_obj, &opts);
  if ( RTEST(opts) != rb_cHash){
    opts = rb_hash_new();
    rb_hash_aset(opts, rb_symname_p("type_flag"), INT2FIX(0));
    rb_hash_aset(opts, rb_symname_p("offset"), INT2NUM(0));
  }
  opts = rb_funcall(self, rb_intern("parse_opts"), 1, opts);

  if (rb_obj_is_kind_of((VALUE)img_obj, rb_cTSKImage)) {
    open_volume_system(self, (VALUE)img_obj, (VALUE)opts);
  } else {
    rb_raise(rb_eTypeError, "Wrong argument type for arg1: (Sleuthkit::Image expected)");
  }
  return self;
}


VALUE open_volume_system(VALUE self, VALUE img_obj, VALUE opts) {

  struct tsk4r_vs_wrapper * vs_ptr;
  Data_Get_Struct(self, struct tsk4r_vs_wrapper, vs_ptr);
  TSK_VS_TYPE_ENUM * vs_type_requested;
  TSK_OFF_T imgaddr = FIX2LONG(rb_hash_aref(opts, rb_symname_p("offset"))); // sector number at which to open vs
  vs_type_requested = get_vs_flag(rb_hash_aref(opts, rb_symname_p("type_flag")));
  
  // open disk image
  struct tsk4r_img * rb_image;
  Data_Get_Struct(img_obj, struct tsk4r_img, rb_image);
  TSK_IMG_INFO * disk = rb_image->image;
  if (disk == NULL) {
    rb_raise(rb_eFatal, "image object had no data.");
  } else {
    // open volume system and assign to data pointer
    TSK_VS_INFO * volume_system = tsk_vs_open(disk, (TSK_DADDR_T)( imgaddr * (TSK_DADDR_T)disk->sector_size ), (TSK_VS_TYPE_ENUM)vs_type_requested);

    vs_ptr->volume = volume_system;
    if (vs_ptr->volume != NULL) {
    
      rb_iv_set(self, "@partition_count", INT2NUM((int)volume_system->part_count));
      rb_iv_set(self, "@volume_system_type", INT2NUM((int)volume_system->vstype));
      rb_iv_set(self, "@description", rb_str_new2( (char *)tsk_vs_type_todesc(volume_system->vstype) ));
      rb_iv_set(self, "@endian", INT2NUM((int)volume_system->endian));
      rb_iv_set(self, "@offset", INT2NUM((int)volume_system->offset));
      rb_iv_set(self, "@block_size", INT2NUM((int)volume_system->block_size));
      rb_iv_set(self, "@parts", volume_get_partitions(self));
      rb_iv_set(self, "@parent", img_obj); // store link to parent system

      return self;
      
    } else {
      rb_warn( "No Volume System found!");
      rb_funcall(self, rb_intern("taint"), 0, NULL);
      return self;
    }
  }
}

VALUE read_volume_system_block(int argc, VALUE *args, VALUE self) {
  VALUE data_string; VALUE addr; VALUE length;
  rb_scan_args(argc, args, "20", &addr, &length);
  // TO DO: ensure arg1 is positive int; arg2 does not exceed num blocks in img.
  TSK_DADDR_T address = NUM2INT(addr);
  size_t number_of_blocks = NUM2INT(length);
  struct tsk4r_vs_wrapper * vs_ptr;
  Data_Get_Struct(self, struct tsk4r_vs_wrapper, vs_ptr);
  TSK_VS_INFO * volume = vs_ptr->volume;
  size_t bytes = number_of_blocks * volume->block_size;
  long size = bytes + 1;
  char buffer[size];
  memset(buffer, '\0', size);
  printf("buffer of %ld created\n", size);
  printf("trying to read address:%llu, number_of_bytes:%ld\n", address, bytes);
  long i = tsk_vs_read_block(volume, address, buffer, bytes);
  printf("%ld bytes successfully read\n", i);
  printf("||%s||\n", buffer);
  long actual_size;
  actual_size = sizeof(buffer);
  printf("buffer was %ld bytes long\n", actual_size);
  data_string = rb_str_new(buffer, size);
  return data_string;
}


// called on VolumeSystem, returns array of VolumePart objects, assigned to @parts
VALUE volume_get_partitions(VALUE self) {
  VALUE part_count = rb_attr_get(self, rb_intern("@partition_count"));
  VALUE part_array = rb_ary_new2(FIX2LONG(part_count));
  long i = 0; VALUE prev = Qnil;
  while (i < FIX2LONG(part_count)) {
    VALUE vpart = rb_funcall(rb_cTSKVolumePart, rb_intern("new"), 2, self, LONG2NUM(i));
    if ( rb_obj_is_kind_of(prev, rb_cTSKVolumePart) ) {
      rb_iv_set(prev, "@next", vpart);
    }
    rb_iv_set(vpart, "@prev", prev);
    prev = vpart;
    rb_ary_push(part_array, vpart);
    i++;
    }
  return part_array;
}

VALUE volume_expose_part_by_idx(VALUE self, VALUE index) {
  VALUE volume_part;
  
  // call VolumePart.new, passing self
  volume_part = rb_funcall(rb_cTSKVolumePart, rb_intern("new"), 2, self, index);
  
  return volume_part;
}

// Sleuthkit::VolumePart functions

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
  
  rb_scan_args(argc, args, "11", &vs_obj, &index);
  if (NIL_P(index)) { index = INT2FIX(0); }
  
  if (rb_obj_is_kind_of((VALUE)vs_obj, rb_cTSKVolumeSystem)) {
    rb_funcall(self, rb_intern("open"), 2, (VALUE)vs_obj, (VALUE)index);
  } else if (rb_obj_is_kind_of((VALUE)vs_obj, rb_cTSKVolumePart)) {
    rb_funcall(self, rb_intern("open"), 2, (VALUE)vs_obj, (VALUE)index);
  }
  else {
    rb_raise(rb_eTypeError, "Wrong argument type for arg1: (Sleuthkit::VolumeSystem expected)");
  }
  
  return self;
}

VALUE open_volume_part(int argc, VALUE *args, VALUE self){
  // self, vs_obj, index, e.g.
  VALUE vs_obj; VALUE index;
  rb_scan_args(argc, args, "12", &vs_obj, &index);
  if (NIL_P(index)) { index = INT2FIX(0); }

  struct tsk4r_vpart_wrapper * partition;
  const TSK_VS_PART_INFO * vp_ptr;
  struct tsk4r_vs_wrapper * parent;  // pointer to parent volume system
  
  if ( ! rb_obj_is_kind_of((VALUE)vs_obj, rb_cTSKVolumeSystem)) {
    rb_raise(rb_eTypeError, "Arg1 is NOT Sleuthkit::VolumeSystem\n");
  }
  
  // open volume system, storing ID locally
  Data_Get_Struct(vs_obj, struct tsk4r_vs_wrapper, parent);
  VALUE parent_id = rb_funcall(vs_obj, rb_intern("object_id"), 0, NULL);
  rb_iv_set(self, "@parent", parent_id);
  
  // open self's struct and assign partition to it
  Data_Get_Struct(self, struct tsk4r_vpart_wrapper, partition);

  TSK_VS_INFO * volume_system = parent->volume;

  TSK_PNUM_T idx = FIX2LONG(index);
  vp_ptr = tsk_vs_part_get(volume_system, idx);
  partition->volume_part = vp_ptr;

  rb_iv_set(self, "@start", INT2NUM((int)vp_ptr->start));
  rb_iv_set(self, "@length", INT2NUM((int)vp_ptr->len));
  rb_iv_set(self, "@description", rb_str_new2( (char *)vp_ptr->desc));
  rb_iv_set(self, "@table_number", INT2NUM((int)vp_ptr->table_num));
  rb_iv_set(self, "@slot_number", INT2NUM((int)vp_ptr->slot_num));
  rb_iv_set(self, "@address", INT2NUM((int)vp_ptr->addr));
  rb_iv_set(self, "@flags", INT2NUM((int)vp_ptr->flags));
  rb_iv_set(self, "@parent", vs_obj); // store link to parent system
  
  return self;
}

VALUE read_partition_data(int argc, VALUE *args, VALUE self) {
  VALUE data_string;
  data_string = rb_str_new2("test");
  return data_string;
}

VALUE read_partition_block(int argc, VALUE *args, VALUE self) {
  VALUE data_string;
  data_string = rb_str_new2("test");
  return data_string;
}




VALUE return_tsk_vol_type_list(int argc, VALUE *args, VALUE self) {
  VALUE io; uint fd;
  rb_scan_args(argc, args, "01", &io);
  if (! rb_obj_is_kind_of(io, rb_cIO) ) {
    rb_warn("Method did not recieve IO object, using STDOUT");
    fd = 1;
  } else {
    fd = FIX2LONG(rb_funcall(io, rb_intern("fileno"), 0));
  }
  
  FILE * hFile = fdopen((int)fd, "w");
  tsk_vs_type_print(hFile);
  fflush(hFile); // clear file buffer, completing write
  
  return self;
}

// helper method to convert ruby integers to TSK_VS_TYPE_ENUM values
TSK_VS_TYPE_ENUM * get_vs_flag(VALUE rb_obj) {
  TSK_VS_TYPE_ENUM * flag;
  switch (TYPE(rb_obj)) {
    case T_STRING:
      //TO DO: convert string to value of Sleuthkit::VolumeSystem::TSK_VS_TYPE_ENUM[string.to_sym]
      flag = (TSK_VS_TYPE_ENUM *)0;
      break;
      
    case T_FIXNUM:
      flag = (TSK_VS_TYPE_ENUM *)NUM2INT(rb_obj);
      break;
      
    case T_SYMBOL:
      // TO DO
      flag = (TSK_VS_TYPE_ENUM *)0;
      break;
      
    default:
      flag = (TSK_VS_TYPE_ENUM *)0;
      break;
  }
  return flag;
}
