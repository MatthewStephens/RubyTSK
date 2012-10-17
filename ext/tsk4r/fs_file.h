/*
 *  fs_file.h: File classes for tsk4r
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


#ifndef RubyTSK_fs_file_h
#define RubyTSK_fs_file_h

#include <tsk3/libtsk.h>
// structures

// Sleuthkit::FileSystemDirectory struct
struct tsk4r_fs_file_wrapper {
  TSK_FS_FILE * file;
};
struct tsk4r_fs_meta_wrapper {
  TSK_FS_META * metadata;
};


// function prototypes
VALUE allocate_fs_file(VALUE self);
VALUE allocate_fs_meta(VALUE self);
VALUE allocate_fs_name(VALUE self);

void deallocate_fs_file(struct tsk4r_fs_file_wrapper * ptr);
void deallocate_fs_meta(struct tsk4r_fs_meta_wrapper * ptr);
void deallocate_fs_name(TSK_FS_NAME * ptr);

VALUE initialize_fs_file(int argc, VALUE *args, VALUE self);
VALUE initialize_fs_meta(int argc, VALUE *args, VALUE self);
VALUE initialize_fs_name(int argc, VALUE *args, VALUE self);

VALUE open_fs_file(int argc, VALUE *args, VALUE self);

VALUE get_meta_from_inum(VALUE self, VALUE filesystem, VALUE addr);
VALUE get_meta_from_file(VALUE self, VALUE fs_file);
VALUE get_meta_from_dir( VALUE self, VALUE fs_dir);
VALUE get_number_of_attributes(VALUE self);


#endif
