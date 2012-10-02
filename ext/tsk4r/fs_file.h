//
//  fs_file.h
//  RubyTSK
//
//  Created by ms3uf on 10/2/12.
//
//  Three TSK structures are wrapped here
//TSK_FS_FILE file content
//TSK_FS_META file metadata entries
//TSK_FS_NAME file name entries


#ifndef RubyTSK_fs_file_h
#define RubyTSK_fs_file_h

#include <tsk3/libtsk.h>
// structures

// Sleuthkit::FileSystemDirectory struct
struct tsk4r_fs_file_wrapper {
  TSK_FS_FILE * file;
};
struct tsk4r_fs_meta_wrapper {
  TSK_FS_META * metadata;
};
struct tsk4r_fs_name_wrapper {
  TSK_FS_NAME * name;
};

// function prototypes
VALUE allocate_fs_file(VALUE self);
VALUE allocate_fs_meta(VALUE self);
VALUE allocate_fs_name(VALUE self);

void deallocate_fs_file(struct tsk4r_fs_file_wrapper * ptr);
void deallocate_fs_meta(struct tsk4r_fs_meta_wrapper * ptr);
void deallocate_fs_name(struct tsk4r_fs_name_wrapper * ptr);

VALUE initialize_fs_file(int argc, VALUE *args, VALUE self);
VALUE initialize_fs_meta(int argc, VALUE *args, VALUE self);
VALUE initialize_fs_name(int argc, VALUE *args, VALUE self);


#endif
