//
//  volume.h
//  RubyTSK
//
//  Created by ms3uf on 9/13/12.
//
//

#ifndef RubyTSK_volume_h
#define RubyTSK_volume_h

#include <tsk3/libtsk.h>

// Sleuthkit::Volume struct
struct tsk4r_vs_wrapper {
  TSK_VS_INFO * volume;
  //  tsk4r_img_wrapper * disk;
};

struct tsk4r_vpart_wrapper {
  const TSK_VS_PART_INFO * volume_part;
};

//VALUE rb_cTSKImage;

// Sleuthkit::VolumeSystem function declarations
VALUE allocate_volume_system(VALUE klass);
void  deallocate_volume_system();
VALUE initialize_volume_system(int argc, VALUE *args, VALUE self);
VALUE open_volume_system(VALUE self, VALUE image_obj, VALUE flag);
static VALUE close_volume_system(VALUE self);
static VALUE read_volume_system_block(int argc, VALUE *args, VALUE self);
static VALUE walk_volume_system(VALUE self);
VALUE volume_expose_part(VALUE self);
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
