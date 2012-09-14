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

struct tsk4r_img {
  TSK_IMG_INFO * image;
  char * fn_given;
};

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
  VALUE * img_obj;
  rb_scan_args(argc, args, "1", &img_obj);
  
  printf("filesystem object init with %d args.\n", argc);
  printf("argument scan complete\n");
  /*
   printf("test1 %d\n", RTEST(rb_respond_to(img_obj, image_open)));
   printf("test2 %d\n", RTEST(rb_respond_to(img_obj, open_volume)));
   printf("test3 %d\n", RTEST(rb_is_instance_id(rb_cTSKImage)));
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


VALUE open_filesystem(VALUE self, VALUE img_obj) {
  printf("open_filesystem here....\n");
  struct tsk4r_fs_wrapper * fs_ptr;
  Data_Get_Struct(self, struct tsk4r_fs_wrapper, fs_ptr);
  struct tsk4r_img * rb_image;
  Data_Get_Struct(img_obj, struct tsk4r_img, rb_image);
  TSK_IMG_INFO * disk = rb_image->image;
  fs_ptr->filesystem = tsk_fs_open_img(disk, 0, TSK_FS_TYPE_DETECT);
  //rb_iv_set(self, "@root_inum", INT2NUM((int)fs_ptr->filesystem->root_inum));
  TSK_INUM_T my_root_inum = 1968;
  TSK_INUM_T my_last_inum = 43;
  TSK_INUM_T my_inum_count = 77;
  TSK_OFF_T my_offset = 43;
  TSK_ENDIAN_ENUM my_endian;
  unsigned int my_block_size = 0;
  if (fs_ptr->filesystem != NULL) {
    my_root_inum = fs_ptr->filesystem->root_inum;
    my_last_inum = fs_ptr->filesystem->last_inum;
    my_block_size = fs_ptr->filesystem->block_size;
    my_endian = fs_ptr->filesystem->endian;
    my_offset = fs_ptr->filesystem->offset;
    my_inum_count = fs_ptr->filesystem->inum_count;
    printf("fs_ptr has root_inum: %d\n", (int)fs_ptr->filesystem->root_inum);
    printf("fs_ptr has last_inum: %d\n", (int)fs_ptr->filesystem->last_inum);
    printf("fs_ptr has block_size: %d\n", (int)fs_ptr->filesystem->block_size);
    printf("fs_ptr has endian: %d\n", (int)fs_ptr->filesystem->endian);
    printf("fs_ptr has offset: %d\n", (int)fs_ptr->filesystem->offset);
    printf("fs_ptr has inum_count: %d\n", (int)fs_ptr->filesystem->inum_count);
  } else {
    my_root_inum = 222;
    my_last_inum = 333;
    my_endian = 666;
  }
  
  rb_iv_set(self, "@root_inum", INT2NUM(my_root_inum));
  rb_iv_set(self, "@last_inum", INT2NUM(my_last_inum));
  rb_iv_set(self, "@block_size", INT2NUM(my_block_size));
  rb_iv_set(self, "@endian", INT2NUM(my_endian));
  rb_iv_set(self, "@offset", INT2NUM(my_offset));
  rb_iv_set(self, "@inum_count", INT2NUM(my_inum_count));
  
  return self;
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