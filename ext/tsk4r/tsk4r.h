/*
 *  sleuthkit.h
 *  sleuthkit
 *
 *  Created by Matthew H. Stephens on 9/14/11.
 *  Copyright 2011 Matthew H. Stephens. All rights reserved.
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
 */

#define VERSION "0.0.4"
#ifndef TSK4R_H
#define TSK4R_H

#include "image.h"
#include "volume.h"
#include "file_system.h"
#include "fs_dir.h"
#include "fs_file.h"

#define MY_TSK_VER TSK_VERSION_NUM
#define OLDER_TSK_VER 0x030202ff

#if MY_TSK_VER < OLDER_TSK_VER
#define TSK4R_HIDE_ADVANCED_FEATURE
#endif


#ifndef TSK4R_HIDE_ADVANCED_FEATURE
#define TSK4R_FS_ATTRS_COUNT 24
#else
#define TSK4R_FS_ATTRS_COUNT 22
#endif

#define TSK4R_BLOCK_ATTRS \
"block_pre_size", \
"block_post_size", 

#define TSK4R_FS_ATTRS_NAME_LIST \
"block_count", \
"block_size", \
"dev_bsize", \
"data_unit_name", \
"endian", \
"first_inum", \
"first_block", \
"flags", \
"fs_id", \
"fs_id_used", \
"ftype", \
"inum_count", \
"isOrphanHunting", \
"journ_inum", \
"last_block", \
"last_block_act", \
"last_inum", \
"list_inum_named", \
"offset", \
"orphan_dir", \
"root_inum", \
"tag"

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


VALUE allocate_image(VALUE klass);

#endif