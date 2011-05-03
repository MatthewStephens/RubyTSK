/*
 *  tsk4r.c
 *  RubyTSK
 *
 *  Created by ms3uf on 5/1/11.
 *  Copyright 2011 Matthew H. Stephens. 
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
 */

#include <stdio.h>
#include <ruby.h>
#include "tsk4r.h"
#include <tsk3/libtsk.h>
//#include "sleuthkit-3.2.1/tsk3/libtsk.h"

static VALUE rb_mTSK;
// static VALUE rb_cTSKBase;

// Defining a space for information and references about the module to be stored internally
VALUE tsk4r = Qnil;

// Prototype for the initialization method - Ruby calls this, not you
void Init_tsk4r();

// Prototype for our method 'test1' - methods are prefixed by 'method_' here
VALUE method_test1(VALUE self);
VALUE method_tsk_version_const(VALUE self);
VALUE method_tsk_version(VALUE self);

// classes

// The initialization method for this module
void Init_tsk4r() {
	rb_mTSK = rb_define_module("TSK");
	rb_define_method(rb_mTSK, "test1", method_test1, 0);
	rb_define_method(rb_mTSK, "tsk_version_const", method_tsk_version_const, 0);
	rb_define_method(rb_mTSK, "tsk_version", method_tsk_version, 0);
	
	// should put tsk version in global constant
	rb_define_const(rb_mTSK, "TSKVersion", rb_str_new2(TSK_VERSION_STR));
	rb_define_const(rb_mTSK, "Version", rb_str_new2(TSK4R_VERSION));

}

// Our 'test1' method.. it simply returns a value of '10' for now.
VALUE method_test1(VALUE self) {
	int x = 10;
	return INT2NUM(x);
}

// these two methods bind to a base constant and a simple function
VALUE method_tsk_version_const(VALUE self) {
	printf("TSK_VERSION_STR: %s\n", TSK_VERSION_STR);
	return TRUE;
}

VALUE method_tsk_version(VALUE self) {
	const char * v = tsk_version_get_str();
	printf("TSK Version: %s\n",  v);
    return 0;
}
