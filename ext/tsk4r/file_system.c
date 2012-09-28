//
//  file_system.c
//  RubyTSK
//
//  Created by ms3uf on 9/14/12.
//
//

#include <stdio.h>
#include <ruby.h>
#include "file_system.h"

extern VALUE rb_cTSKImage;
extern VALUE rb_cTSKVolumeSystem;
extern VALUE rb_cTSKVolumePart;

//struct tsk4r_img {
//  TSK_IMG_INFO * image;
//  char * fn_given;
//};
// prototypes (private)
TSK_FS_TYPE_ENUM * get_fs_flag();

// functions

void deallocate_filesystem(struct tsk4r_fs_wrapper * ptr){
  TSK_FS_INFO *filesystem = ptr->filesystem;
  tsk_fs_close(filesystem);
  xfree(ptr);
}



VALUE allocate_filesystem(VALUE klass){
  printf("allocate_filesystem starting...\n");
  struct tsk4r_fs_wrapper * ptr;
  //    ptr = ALLOC(struct tsk4r_fs_wrapper);
  printf("finishing allocation.\n");
  return Data_Make_Struct(klass, struct tsk4r_fs_wrapper, 0, deallocate_filesystem, ptr);
}


VALUE initialize_filesystem(int argc, VALUE *args, VALUE self){
  VALUE * source_obj; VALUE flag;

  rb_scan_args(argc, args, "1", &source_obj, &flag);
  if (NIL_P(flag)) { flag = INT2NUM(0); }

  open_filesystem(self, (VALUE)source_obj, (VALUE)flag);
  
  return self;
}


VALUE open_filesystem(VALUE self, VALUE parent_obj, VALUE fs_type_flag) {
  printf("open_filesystem here....\n");
  struct tsk4r_fs_wrapper * fs_ptr;
  TSK_FS_TYPE_ENUM * type_flag_num = get_fs_flag(fs_type_flag);
  
  Data_Get_Struct(self, struct tsk4r_fs_wrapper, fs_ptr);
  
  if (rb_obj_is_kind_of((VALUE)parent_obj, rb_cTSKImage)) {
    printf("received image object.\n");

    struct tsk4r_img * rb_image;
    TSK_OFF_T offset = 0;
    Data_Get_Struct(parent_obj, struct tsk4r_img, rb_image);
    TSK_IMG_INFO * disk = rb_image->image;
    fs_ptr->filesystem = tsk_fs_open_img(disk, offset, (TSK_FS_TYPE_ENUM)type_flag_num);

  } else if (rb_obj_is_kind_of((VALUE)parent_obj, rb_cTSKVolumeSystem)) {
    printf("received volume object.\n");

    struct tsk4r_vs * rb_volumesystem;
    Data_Get_Struct(parent_obj, struct tsk4r_vs, rb_volumesystem);

    TSK_PNUM_T c = 0;
    while (c < rb_volumesystem->volume->part_count) {
      const TSK_VS_PART_INFO * partition = tsk_vs_part_get(rb_volumesystem->volume, c);
      fs_ptr->filesystem = tsk_fs_open_vol(partition, (TSK_FS_TYPE_ENUM)type_flag_num);
      if (fs_ptr->filesystem != NULL) { break; } else { printf("failed on index %d\n", c); }
      c++;
    }
  } else if (rb_obj_is_kind_of((VALUE)parent_obj, rb_cTSKVolumePart)) {
    printf("received partition object.\n");

    struct tsk4r_vs_part * rb_partition;
    Data_Get_Struct(parent_obj, struct tsk4r_vs_part, rb_partition);
    fs_ptr->filesystem = tsk_fs_open_vol(rb_partition->volume_part, (TSK_FS_TYPE_ENUM)type_flag_num);
  } else {
    rb_raise(rb_eTypeError, "arg1 must be a SleuthKit::Image, SleuthKit::VolumeSystem or SleuthKit::VolumePart object.");
  }
  
  if (fs_ptr->filesystem != NULL) {

    VALUE my_description = get_filesystem_type(self);
    rb_iv_set(self, "@description", my_description);
    rb_iv_set(self, "@data_unit_name", rb_str_new2(fs_ptr->filesystem->duname));
    rb_iv_set(self, "@root_inum", INT2NUM((int)fs_ptr->filesystem->root_inum));
    rb_iv_set(self, "@last_inum", INT2NUM((int)fs_ptr->filesystem->last_inum));
    rb_iv_set(self, "@block_size", INT2NUM((int)fs_ptr->filesystem->block_size));
    rb_iv_set(self, "@endian", INT2NUM((int)fs_ptr->filesystem->endian));
    rb_iv_set(self, "@offset", INT2NUM((int)fs_ptr->filesystem->offset));
    rb_iv_set(self, "@inum_count", INT2NUM((int)fs_ptr->filesystem->inum_count));
    
  } else {
    rb_warn("filesystem pointer is NULL\n");
  }
  return self;
}

VALUE open_filesystem_from_img(VALUE self, VALUE img_obj) {
  printf("open_filesystem from here....\n");
  struct tsk4r_fs_wrapper * fs_ptr;
  struct tsk4r_img * rb_image;
  TSK_OFF_T offset = 0;
  Data_Get_Struct(self, struct tsk4r_fs_wrapper, fs_ptr);
  Data_Get_Struct(img_obj, struct tsk4r_img, rb_image);
  TSK_IMG_INFO * image = rb_image->image;
  fs_ptr->filesystem = tsk_fs_open_img(image, offset, TSK_FS_TYPE_DETECT);
  return self;
}

VALUE get_filesystem_type(VALUE self) {
  const char * mytype;
  struct tsk4r_fs_wrapper * fs_ptr;
  Data_Get_Struct(self, struct tsk4r_fs_wrapper, fs_ptr);
  mytype = tsk_fs_type_toname(fs_ptr->filesystem->ftype);
  rb_iv_set(self, "@name", rb_str_new2(mytype));
  return rb_str_new2(mytype);
}

VALUE open_filesystem_from_vol(VALUE self, VALUE vol_obj) {
  printf("open filesystem_from_vol here...\n");
  struct tsk4r_fs_wrapper * fs_ptr;
  Data_Get_Struct(self, struct tsk4r_fs_wrapper, fs_ptr);
  struct tsk4r_vs * rb_volume;
  Data_Get_Struct(vol_obj, struct tsk4r_vs, rb_volume);
  //    TSK_VS_INFO * volume = rb_volume->volume;
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

// helper method to convert ruby integers to TSK_IMG_TYPE_ENUM values
TSK_FS_TYPE_ENUM * get_fs_flag(VALUE rb_obj) {
  TSK_FS_TYPE_ENUM * flag;
  switch (TYPE(rb_obj)) {
    case T_STRING:
      printf("string is %s\n", StringValuePtr(rb_obj));
      char *str = StringValuePtr(rb_obj);
      //TO DO: convert string to value of Sleuthkit::FileSystem::TSK_FS_TYPE_ENUM[string.to_sym]
      printf("flag is %s\n", str);
      flag = (TSK_FS_TYPE_ENUM *)0;
      break;
      
    case T_FIXNUM:
      printf("fs_type is %ld\n", NUM2INT(rb_obj));
      long num = NUM2INT(rb_obj);
      flag = (TSK_FS_TYPE_ENUM *)num;
      printf("flag is %ld\n", num);
      break;
      
    case T_SYMBOL:
      // TO DO
      flag = (TSK_FS_TYPE_ENUM *)0;
      break;
      
    default:
      flag = (TSK_FS_TYPE_ENUM *)0;
      break;
  }
  return flag;
}