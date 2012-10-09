//
//  tsk4r_i.h
//  RubyTSK
//
//  Created by ms3uf on 10/9/12.
//
//

#ifndef RubyTSK_tsk4r_i_h
#define RubyTSK_tsk4r_i_h

#define MY_TSK_VER 0x030203ff
#define OLDER_TSK_VER 0x030202ff

#if MY_TSK_VER < OLDER_TSK_VER
#define TSK4R_HIDE_ADVANCED_FEATURE
#endif

#ifndef TSK4R_HIDE_ADVANCED_FEATURE
#define TSK4R_FS_ATTRS_COUNT 24
#else
#define TSK4R_FS_ATTRS_COUNT 22
#endif

#define TSK4R_BLOCK_ATTRS \
"block_pre_size", \
"block_post_size",

#define TSK4R_FS_ATTRS_NAME_LIST \
"block_count", \
"block_size", \
"dev_bsize", \
"data_unit_name", \
"endian", \
"first_inum", \
"first_block", \
"flags", \
"fs_id", \
"fs_id_used", \
"ftype", \
"inum_count", \
"isOrphanHunting", \
"journ_inum", \
"last_block", \
"last_block_act", \
"last_inum", \
"list_inum_named", \
"offset", \
"orphan_dir", \
"root_inum", \
"tag"



#endif
