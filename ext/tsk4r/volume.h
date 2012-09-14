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

//VALUE rb_cTSKImage;

// Sleuthkit::Volume function declarations
VALUE allocate_volume();
void  deallocate_volume();
VALUE initialize_volume(int argc, VALUE *args, VALUE self);
VALUE open_volume(VALUE self, VALUE image_obj);
static VALUE close_volume(VALUE self);
static VALUE read_volume_block(int argc, VALUE *args, VALUE self);
static VALUE walk_volume(VALUE self);

#endif
