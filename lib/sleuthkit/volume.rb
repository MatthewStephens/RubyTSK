# -*- coding: utf-8 -*-
module Sleuthkit
  class VolumeSystem
    include ::Sleuthkit
    private
    def parse_opts(opts)
      presets = { :offset => 0, :type_flag => 0 }      
      presets.each_pair do |key, val|
        unless opts.has_key?(key) then opts[key] = val end
      end
      return opts
    end
  end
end
