//
//  file_system.h
//  RubyTSK
//
//  Created by Matthew Stephens on 9/14/12.
//
//

#ifndef RubyTSK_file_system_h
#define RubyTSK_file_system_h

#include <tsk3/libtsk.h>

// Sleuthkit::FileSystem struct
struct tsk4r_fs_wrapper {
  TSK_FS_INFO * filesystem;
};
// Sleuthkit::Volume struct
struct tsk4r_vs {
  TSK_VS_INFO * volume;
  //  tsk4r_img_wrapper * disk;
};
struct tsk4r_vs_part {
  TSK_VS_PART_INFO * volume_part;
};
struct tsk4r_img {
  TSK_IMG_INFO * image;
};

// Sleuthkit::FileSystem
VALUE initialize_filesystem(int argc, VALUE *args, VALUE self);
VALUE allocate_filesystem(VALUE self);
void  deallocate_filesystem(struct tsk4r_fs_wrapper * ptr);
VALUE open_filesystem(VALUE self, VALUE source, VALUE opts);
VALUE open_fs_from_image(VALUE self, VALUE image_obj, VALUE opts);
VALUE open_fs_from_volume(VALUE self, VALUE vol_obj, VALUE opts);
VALUE open_fs_from_partition(VALUE self, VALUE partition, VALUE opts);
VALUE get_filesystem_type(VALUE self);
VALUE call_tsk_fsstat(VALUE self, VALUE io);
VALUE open_directory_by_name(int argc, VALUE *args, VALUE self);
VALUE open_directory_by_inum(int argc, VALUE *args, VALUE self);

#endif
