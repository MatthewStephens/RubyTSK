//
//  image.h
//  RubyTSK
//
//  Created by Matthew Stephens on 9/13/12.
//
//

#ifndef RubyTSK_image_h
#define RubyTSK_image_h

#include <tsk3/libtsk.h>


// Sleuthkit::Image struct-in-ruby-object
struct tsk4r_img_wrapper {
  TSK_IMG_INFO * image;
};

// Sleuthkit::Image function declarations
VALUE allocate_image(VALUE klass);
void  deallocate_image(struct tsk4r_img_wrapper * ptr);
VALUE initialize_disk_image(int argc, VALUE *args, VALUE self);
VALUE image_open(VALUE self, VALUE filename_str, VALUE disk_type);
VALUE image_size(VALUE self);
VALUE image_type_to_desc(TSK_IMG_TYPE_ENUM num);
VALUE image_type_to_name(TSK_IMG_TYPE_ENUM num);



#endif
