# -*- coding: utf-8 -*-
require 'sleuthkit/version'
require 'tsk4r/tsk4r'

module Sleuthkit
  # tell us about yourself
	def self.version_string
		"Sleuthkit Ruby Binding version #{Sleuthkit::VERSION} (TSK version: #{Sleuthkit::TSK_VERSION})"
	end
  def [](sym)
    self.instance_variable_get("@#{sym.to_s}")
  end
  def inspect_object
    h = Hash.new
    self.instance_variables.each do |var|
      h[var.gsub(/@/, '').to_sym] = instance_variable_get(var)
    end
    pp h
    return self
  end

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

    def number_of_sectors
      div = self.size / self.sector_size
      mod = self.size % self.sector_size
      if mod > 0
        @number_of_sectors = div + 1
      else
        @number_of_sectors = div
      end
    end
    def pure_ruby(n)
      puts "HEY, we got method called from C but written in Ruby!!\n"
    end


	end

  class VolumeSystem
    include ::Sleuthkit
  end
  class VolumePart
    include ::Sleuthkit
  end
  class FileSystem
    include ::Sleuthkit

    def tsk_fsstat(file)
      file.puts 'Hi Matt!'
    end
    def print_tsk_fsstat(report = "")
      # require 'ruby-debug'; debugger
      
      if report.kind_of?( IO )
        self.call_tsk_fsstat(report)
      elsif report.kind_of?(String)
        r, w = IO.pipe
        self.call_tsk_fsstat(w)
        report << r.read
        r.close
        return report
      else
        raise ArgumentError, "arg1 should be IO, File or String object."
      end
    end
  end

end
