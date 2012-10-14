/*
 *  tsk4r.h
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

#define VERSION "0.0.4"
#ifndef TSK4R_H
#define TSK4R_H

#include "tsk4r_i.h"
#include "image.h"
#include "volume.h"
#include "file_system.h"
#include "fs_dir.h"
#include "fs_file.h"
#include "fs_block.h"


const char * TSK4R_FS_ATTRS_NAMES[TSK4R_FS_ATTRS_COUNT] = {
#ifndef TSK4R_HIDE_ADVANCED_FEATURE
TSK4R_BLOCK_ATTRS
#endif
  TSK4R_FS_ATTRS_NAME_LIST
};


// Prototype for the initialization method - Ruby calls this, not you
void Init_tsk4r();
void klassify(VALUE obj, char * name);


// Prototype for the module and classes directly below it
VALUE rb_mtsk4r;
VALUE rb_mtsk4r_v;
VALUE rb_mtsk4r_fs;

VALUE rb_cTSKImage;
VALUE rb_cTSKVolumeSystem;
VALUE rb_cTSKVolumePart;
VALUE rb_cTSKFileSystem;
VALUE rb_cTSKFileSystemDir;
VALUE rb_cTSKFileSystemFileData;
VALUE rb_cTSKFileSystemFileMeta;
VALUE rb_cTSKFileSystemFileName;
VALUE rb_cTSKFileSystemBlock;


VALUE allocate_image(VALUE klass);

#endif