//
//  fs_dir.h
//  RubyTSK
//
//  Created by Matthew Stephens on 10/1/12.
//
//

#ifndef RubyTSK_fs_dir_h
#define RubyTSK_fs_dir_h

#include <tsk3/libtsk.h>

// Sleuthkit::FileSystemDirectory struct
struct tsk4r_fs_dir_wrapper {
  TSK_FS_DIR * directory;
};

// Sleuthkit::FileSystemDirectory

VALUE allocate_fs_dir(VALUE self);
void  deallocate_fs_dir(struct tsk4r_fs_dir_wrapper * ptr);
VALUE initialize_fs_dir(int argc, VALUE *args, VALUE self);
VALUE open_fs_directory(VALUE self, VALUE parent_obj, VALUE name_or_inum, VALUE opts);

#endif
