/*
 *  image.h: Image class for tsk4r
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
VALUE image_type_to_desc(TSK_IMG_TYPE_ENUM num);
VALUE image_type_to_name(TSK_IMG_TYPE_ENUM num);



#endif
