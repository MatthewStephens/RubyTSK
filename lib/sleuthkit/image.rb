# -*- coding: utf-8 -*-
module Sleuthkit
  class Image
    include Enumerable
    include ::Sleuthkit
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
    # a little sleight of hand to add Ruby code to the #intialize written in C
    # alias_method :old_new, :new
    # def new(*args)
    #   puts "Ruby before C!!"
    #   old_new(*args)
    # end
  

    def number_of_sectors
      div = self.size / self.sector_size
      mod = self.size % self.sector_size
      if mod > 0
        @number_of_sectors = div + 1
      else
        @number_of_sectors = div
      end
    end
    # private
    def parse_opts(h={})
      opts = h || Hash.new
      if h.kind_of?(Fixnum) then opts = {:type_flag => h} end
      presets = { :offset => 0, :type_flag => 0 }      
      presets.each_pair do |key, val|
        unless opts.has_key?(key) then opts[key] = val end
      end
      opts.each_pair do |k,v| puts " #{k}, #{v}" end
      return opts
    end

  end
end