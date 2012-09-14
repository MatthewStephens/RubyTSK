/*
 *  sleuthkit.h
 *  sleuthkit
 *
 *  Created by Matthew H. Stephens on 9/14/11.
 *  Copyright 2011 Matthew H. Stephens. All rights reserved.
 *
 */
#define VERSION "0.0.2"
#ifndef TSK4R_H
#define TSK4R_H

#include "image.h"
#include "volume.h"
#include "file_system.h"

// Prototype for the initialization method - Ruby calls this, not you
void Init_tsk4r();

// Prototype for the module and classes directly below it
VALUE rb_mtsk4r;
VALUE rb_cTSKImage;
VALUE rb_cTSKVolume;
VALUE rb_cTSKFileSystem;

VALUE allocate_image(VALUE klass);

#endif