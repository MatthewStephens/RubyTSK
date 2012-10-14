//
//  fs_attr.c
//  RubyTSK
//
//  Created by ms3uf on 10/14/12.
//
//

#include <stdio.h>
#include <ruby.h>
#include "fs_attr.h"
#include "fs_file.h"

extern VALUE rb_cTSKFileSystemFileData;

VALUE allocate_fs_attr(VALUE klass) {
  TSK_FS_ATTR * ptr;
  return Data_Make_Struct(klass, TSK_FS_ATTR, 0, deallocate_fs_attr, ptr);
}

void deallocate_fs_attr(TSK_FS_ATTR * ptr) {
  xfree(ptr);
}

VALUE initialize_fs_attr(int argc, VALUE *args, VALUE self) {
  VALUE fs_file; VALUE attr_idx; char * method;
  rb_scan_args(argc, args, "11", &fs_file, &attr_idx);
  if (attr_idx == Qnil) {
    attr_idx = INT2NUM(0); method="fetch_default_attribute";
  } else {
    method = "fetch_attribute_by_index";
  }
  rb_funcall(self, rb_intern(method), 2, fs_file, attr_idx);
  
  return self;
}

VALUE fetch_attr(int argc, VALUE *args, VALUE self) {
  VALUE fs_file; VALUE idx; int a_idx;
  rb_scan_args(argc, args, "11", &fs_file, &idx);
  if (! rb_obj_is_kind_of(fs_file, rb_cTSKFileSystemFileData)) {
    rb_raise(rb_eArgError, "arg1 must be a Sleuthkit::FileSystem::FileData object!");
  }
  if (! rb_obj_is_kind_of(idx, rb_cInteger)) {
    rb_warn("arg2 should be Integer.");
  }
  if (idx == Qnil) {
    idx = INT2NUM(0);
  } else if (! rb_obj_is_kind_of(idx, rb_cInteger)) {
    if (rb_obj_respond_to(self, rb_intern("to_i"), 0)) {
      idx = rb_funcall2(idx, rb_intern("to_i"), 0, NULL);
    } else {
      idx = INT2NUM(0);
    }
  }
  a_idx = (int)NUM2LONG(idx);
  const TSK_FS_ATTR * ptr; struct tsk4r_fs_file_wrapper * fs;
  Data_Get_Struct(self, TSK_FS_ATTR, ptr);
  Data_Get_Struct(fs_file, struct tsk4r_fs_file_wrapper, fs);

  TSK_FS_FILE * f;
  f = fs->file;
  
  ptr = tsk_fs_file_attr_get_idx(f, a_idx);
  
  if (ptr != NULL) {
    rb_iv_set(self, "@file", fs_file);
    rb_iv_set(self, "@flags", ULONG2NUM(ptr->flags));
    rb_iv_set(self, "@id", ULONG2NUM(ptr->id));
    if (ptr->name) {
      rb_iv_set(self, "@name", rb_str_new2(ptr->name));
    }
    rb_iv_set(self, "@name_size", INT2NUM(ptr->name_size));
    rb_iv_set(self, "@size", ULONG2NUM(ptr->size));
    rb_iv_set(self, "@type", INT2NUM(ptr->type));
  } else {
    rb_warn("unable to get attribute");
  }
  return self;
}