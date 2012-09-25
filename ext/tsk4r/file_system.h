//
//  file_system.h
//  RubyTSK
//
//  Created by ms3uf on 9/14/12.
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
VALUE open_filesystem(VALUE self, VALUE image_obj);
VALUE open_filesystem_from_vol(VALUE self, VALUE vol_obj);
VALUE open_filesystem_from_img(VALUE self, VALUE image_obj);
VALUE close_filesystem(VALUE self);
VALUE get_filesystem_type(VALUE self);

#endif
