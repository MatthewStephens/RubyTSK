# -*- coding: utf-8 -*-
require 'sleuthkit/version'
require 'tsk4r/tsk4r'

module Sleuthkit
  # tell us about yourself
	def self.version_string
		"Sleuthkit Ruby Binding version #{Sleuthkit::VERSION}"
	end	
	class Image
    include Enumerable
    TSK_IMG_TYPE_ENUM = {
      :TSK_IMG_TYPE_DETECT => 0x0000,   # Use autodetection methods
      
      :TSK_IMG_TYPE_RAW_SING => 0x0001, # Raw single disk image
      :TSK_IMG_TYPE_RAW_SPLIT => 0x0002,        # Raw split image
        
      :TSK_IMG_TYPE_AFF_AFF => 0x0004,  # AFF AFF Format
      :TSK_IMG_TYPE_AFF_AFD => 0x0008,  # AFD AFF Format
      :TSK_IMG_TYPE_AFF_AFM => 0x0010,  # AFM AFF Format
      :TSK_IMG_TYPE_AFF_ANY => 0x0020,  # Any format supported by AFFLIB (including beta ones)
        
      :TSK_IMG_TYPE_EWF_EWF => 0x0040,  # EWF version
        
      :TSK_IMG_TYPE_UNSUPP => 0xffff,   # Unsupported disk image type
    }
	end
end
