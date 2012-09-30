//
//  file_system.c
//  RubyTSK
//
//  Created by Matthew Stephens on 9/14/12.
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

  rb_scan_args(argc, args, "11", &source_obj, &flag);
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
    rb_iv_set(self, "@block_count", ULONG2NUM((unsigned long long)fs_ptr->filesystem->block_count));
//    rb_iv_set(self, "@block_getflags", INT2NUM((int)fs_ptr->filesystem->block_getflags)); // do not impl
    rb_iv_set(self, "@block_post_size", UINT2NUM((uint)fs_ptr->filesystem->block_post_size));
    rb_iv_set(self, "@block_pre_size", UINT2NUM((uint)fs_ptr->filesystem->block_pre_size));
    rb_iv_set(self, "@block_size", UINT2NUM((uint)fs_ptr->filesystem->block_size));
//    rb_iv_set(self, "@block_walk", fs_ptr->filesystem->block_walk);// no
    rb_iv_set(self, "@dev_bsize", UINT2NUM((uint)fs_ptr->filesystem->dev_bsize));
    rb_iv_set(self, "@data_unit_name", rb_str_new2(fs_ptr->filesystem->duname));
    rb_iv_set(self, "@endian", UINT2NUM((uint)fs_ptr->filesystem->endian));
    rb_iv_set(self, "@first_inum", ULONG2NUM((unsigned long int)fs_ptr->filesystem->first_inum));
//    rb_iv_set(self, "@file_add_meta", fs_ptr->filesystem->file_add_meta); // do not impl
    rb_iv_set(self, "@first_block", ULONG2NUM((unsigned long int)fs_ptr->filesystem->first_block));
    rb_iv_set(self, "@flags", UINT2NUM((uint)fs_ptr->filesystem->flags));
//    rb_iv_set(self, "@fread_owner_sid", fs_ptr->filesystem->fread_owner_sid); // do not impl
    rb_iv_set(self, "@fs_id", rb_str_new2((char *)fs_ptr->filesystem->fs_id));
    rb_iv_set(self, "@fs_id_used", UINT2NUM((uint)fs_ptr->filesystem->fs_id_used));
//    rb_iv_set(self, "@fscheck", fs_ptr->filesystem->fscheck); // do not impl
//    rb_iv_set(self, "@fsstat", fs_ptr->filesystem->fsstat); // do not impl
    rb_iv_set(self, "@ftype", UINT2NUM((uint)fs_ptr->filesystem->ftype));
//    rb_iv_set(self, "@get_default_attr_type", fs_ptr->filesystem->get_default_attr_type); // do not impl
//    rb_iv_set(self, "@inode_walk", fs_ptr->filesystem->inode_walk);//no
    rb_iv_set(self, "@inum_count", ULONG2NUM((unsigned long int)fs_ptr->filesystem->inum_count));
    rb_iv_set(self, "@isOrphanHunting", UINT2NUM((uint)fs_ptr->filesystem->isOrphanHunting));
//    rb_iv_set(self, "@istat", fs_ptr->filesystem->istat); // do not impl
//    rb_iv_set(self, "@jblk_walk", fs_ptr->filesystem->jblk_walk);//no
//    rb_iv_set(self, "@jentry_walk", fs_ptr->filesystem->jentry_walk);//no
//    rb_iv_set(self, "@jopen", fs_ptr->filesystem->jopen);//no
    rb_iv_set(self, "@journ_inum", ULONG2NUM((unsigned long int)fs_ptr->filesystem->journ_inum));
    rb_iv_set(self, "@last_block", ULONG2NUM((unsigned long int)fs_ptr->filesystem->last_block));
    rb_iv_set(self, "@last_block_act", ULONG2NUM((unsigned long int)fs_ptr->filesystem->last_block_act));
    rb_iv_set(self, "@last_inum", ULONG2NUM((unsigned long int)fs_ptr->filesystem->last_inum));
//    rb_iv_set(self, "@list_inum_named", fs_ptr->filesystem->list_inum_named); // to do
//    rb_iv_set(self, "@load_attrs", fs_ptr->filesystem->load_attrs); // do not impl
//    rb_iv_set(self, "@name_cmp", fs_ptr->filesystem->name_cmp); // do no impl
    rb_iv_set(self, "@offset", LONG2NUM((long int)fs_ptr->filesystem->offset));
//    rb_iv_set(self, "@orphan_dir", fs_ptr->filesystem->orphan_dir); // to do
    rb_iv_set(self, "@root_inum", ULONG2NUM((unsigned long int)fs_ptr->filesystem->root_inum));
    rb_iv_set(self, "@tag", INT2NUM(fs_ptr->filesystem->tag));
    
    rb_iv_set(self, "@description", my_description);
    rb_iv_set(self, "@parent", parent_obj);
    
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

// want to call this function from ruby
// uint8_t(*fsstat) (TSK_FS_INFO * fs, FILE * hFile);
VALUE call_tsk_fsstat(VALUE self, VALUE io){
  if (! rb_obj_is_kind_of(io, rb_cIO) ) {
    rb_raise(rb_eArgError, "Method did not recieve IO object");
  }
  int fd = FIX2LONG(rb_funcall(io, rb_intern("fileno"), 0));
  FILE * hFile = fdopen((int)fd, "w");

  struct tsk4r_fs_wrapper * fs_ptr;
  Data_Get_Struct(self, struct tsk4r_fs_wrapper, fs_ptr);
  
  // this accesses the function pointer in TSK_FS_INFO
  // the function dumps a status report (text)
  // to the file handle specified by hFile
  if (fs_ptr->filesystem != NULL) {
    uint8_t(*myfunc) (TSK_FS_INFO * fs, FILE * hFile);
    myfunc = fs_ptr->filesystem->fsstat;
    int r = myfunc(fs_ptr->filesystem, hFile);
    fflush(hFile); // clear file buffer, completing write
    if (r != 0 ) { rb_raise(rb_eRuntimeError, "TSK function: fsstat exited with an error."); }
  }

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