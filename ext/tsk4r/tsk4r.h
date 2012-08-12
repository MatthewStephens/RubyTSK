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

// Prototype for the initialization method - Ruby calls this, not you
void Init_tsk4r();

// Prototype for the method and classes directly below it
VALUE rb_mtsk4r;
VALUE rb_cClass1;
VALUE rb_cClass2;
VALUE rb_cClass3;

#endif