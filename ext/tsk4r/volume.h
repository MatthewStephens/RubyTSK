/*
 *  volume.h: Volume module for tsk4r
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

#ifndef RubyTSK_volume_h
#define RubyTSK_volume_h

#include <tsk3/libtsk.h>

// Sleuthkit::Volume struct
struct tsk4r_vs_wrapper {
  TSK_VS_INFO * volume;
};

struct tsk4r_vpart_wrapper {
  const TSK_VS_PART_INFO * volume_part;
};


// Sleuthkit::VolumeSystem function declarations
VALUE allocate_volume_system(VALUE klass);
void  deallocate_volume_system();
VALUE initialize_volume_system(int argc, VALUE *args, VALUE self);
VALUE open_volume_system(VALUE self, VALUE image_obj, VALUE options);
static VALUE close_volume_system(VALUE self);
static VALUE read_volume_system_block(int argc, VALUE *args, VALUE self);
static VALUE walk_volume_system(VALUE self);
VALUE volume_expose_part_by_idx(VALUE self, VALUE index);
VALUE volume_get_partitions(VALUE self);

// Sleuthkit::VolumePart function declarations
VALUE allocate_volume_part(VALUE klass);
void  deallocate_volume_part();
VALUE initialize_volume_part(int argc, VALUE *args, VALUE self);
VALUE open_volume_part(int argc, VALUE *args, VALUE self);
//static VALUE close_volume_part(VALUE self);
//VALUE read_volume_part_block(int argc, VALUE *args, VALUE self);
//static VALUE walk_volume_part(VALUE self);

#endif
