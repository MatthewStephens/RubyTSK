/*
 *  file_system.h: File System classes for tsk4r
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

#ifndef RubyTSK_file_system_h
#define RubyTSK_file_system_h

#include <tsk3/libtsk.h>
#include "tsk4r_i.h"

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
VALUE open_file_by_name(int argc, VALUE *args, VALUE self);
VALUE open_file_by_inum(int argc, VALUE *args, VALUE self);

#endif
