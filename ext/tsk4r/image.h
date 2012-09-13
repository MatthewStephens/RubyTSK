//
//  image.h
//  RubyTSK
//
//  Created by ms3uf on 9/13/12.
//
//

#ifndef RubyTSK_image_h
#define RubyTSK_image_h

#include <tsk3/libtsk.h>

// Sleuthkit::Image struct
struct tsk4r_img_wrapper {
  TSK_IMG_INFO * image;
  char * fn_given;
};

// Sleuthkit::Image
VALUE allocate_image(VALUE klass);
void  deallocate_image(struct tsk4r_img_wrapper * ptr);
VALUE initialize_disk_image(int argc, VALUE *args, VALUE self);
VALUE image_open(VALUE self, VALUE filename_str);
VALUE image_size(VALUE self);
VALUE sector_size(VALUE self);



#endif
