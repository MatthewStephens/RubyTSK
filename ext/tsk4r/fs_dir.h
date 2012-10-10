/*
 *  fs_dir.h: Directory classes for tsk4r
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

#ifndef RubyTSK_fs_dir_h
#define RubyTSK_fs_dir_h

#include <tsk3/libtsk.h>

// Sleuthkit::FileSystemDirectory struct
struct tsk4r_fs_dir_wrapper {
  TSK_FS_DIR * directory;
};

// Sleuthkit::FileSystemDirectory

VALUE allocate_fs_dir(VALUE self);
void  deallocate_fs_dir(struct tsk4r_fs_dir_wrapper * ptr);
VALUE initialize_fs_dir(int argc, VALUE *args, VALUE self);
VALUE open_fs_directory(VALUE self, VALUE parent_obj, VALUE name_or_inum, VALUE opts);

#endif
